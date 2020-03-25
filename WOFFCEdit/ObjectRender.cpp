#include "pch.h"
#include "ObjectRender.h"
#include "DisplayObject.h"
#include "Game.h"
#include <string>

using namespace DirectX; 
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr; 

ObjectRender::ObjectRender()
{
	opened = false; 

	m_deviceResources = std::make_unique<DX::DeviceResources>(); 
	m_deviceResources->RegisterDeviceNotify(this); 
	m_displayList.clear(); 

	object_index = 0; 

	// Camera
	m_camPosition.x = -5.0f; 
	m_camPosition.y = 5.0f; 
	m_camPosition.z = 5.0f;

	m_camOrientation.x = 0; 
	m_camOrientation.y = 0; 
	m_camOrientation.z = 0; 

	m_camLookAt.x = 0.0f; 
	m_camLookAt.y = 0.0f; 
	m_camLookAt.z = 0.0f; 

	m_camLookDirection.x = 0.0f; 
	m_camLookDirection.y = 0.0f; 
	m_camLookDirection.z = 0.0f; 

	m_camRight.x = 0.0f;
	m_camRight.y = 0.0f;
	m_camRight.z = 0.0f;

	m_camOrientation.x = 0.0f;
	m_camOrientation.y = 0.0f;
	m_camOrientation.z = 0.0f;
}

ObjectRender::~ObjectRender()
{
}

void ObjectRender::Initialize(HWND window, int width, int height)
{
	opened = true; 

	m_deviceResources->SetWindow(window, width, height); 

	m_deviceResources->CreateDeviceResources(); 
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources(); 
	CreateWindowSizeDependentResources();
}

void ObjectRender::Tick(int objectIndex)
{
	object_index = objectIndex;

	if (opened)
	{
		m_timer.Tick([&]()
		{
			Update();
		});

		Render();
	}
}

void ObjectRender::Update()
{
	m_view = Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY); 
	
	m_batchEffect->SetView(m_view); 
	m_batchEffect->SetWorld(Matrix::Identity); 
}

void ObjectRender::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	Clear();

	m_deviceResources->PIXBeginEvent(L"Render");
	auto context = m_deviceResources->GetD3DDeviceContext();

	//RENDER OBJECTS FROM SCENEGRAPH
	m_deviceResources->PIXBeginEvent(L"Draw model");
	const XMVECTORF32 scale = { m_displayList[object_index].m_scale.x, m_displayList[object_index].m_scale.y, m_displayList[object_index].m_scale.z };
	const XMVECTORF32 translate = { m_displayList[object_index].m_position.x, m_displayList[object_index].m_position.y, m_displayList[object_index].m_position.z };

	//convert degrees into radians for rotation matrix
	XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(m_displayList[object_index].m_orientation.y *3.1415 / 180,
		m_displayList[object_index].m_orientation.x *3.1415 / 180,
		m_displayList[object_index].m_orientation.z *3.1415 / 180);

	XMMATRIX local = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);

	m_displayList[object_index].m_model->Draw(context, *m_states, local, m_view, m_projection, false);	//last variable in draw,  make TRUE for wireframe

	m_deviceResources->PIXEndEvent();

	m_deviceResources->Present();
}

void ObjectRender::InitModels(std::vector<ModelInfo>* models)
{
	auto device = m_deviceResources->GetD3DDevice(); 

	if (!m_displayList.empty())
	{
		m_displayList.clear(); 
	}

	int numObjects = models->size();
	
	for (int i = 0; i < numObjects; i++)
	{
		DisplayObject newDisplayObject; 

		// Load model
		std::wstring modelwstr = StringToWCHART(models->at(i).model_path);
		newDisplayObject.m_model = Model::CreateFromCMO(device, modelwstr.c_str(), *m_fxFactory, true);

		// Load texture
		std::wstring texturewstr = StringToWCHART(models->at(i).tex_diffuse_path);
		HRESULT rs;
		rs = CreateDDSTextureFromFile(device, texturewstr.c_str(), nullptr, &newDisplayObject.m_texture_diffuse);

		// If texture fails. Load error default
		if (rs)
		{
			CreateDDSTextureFromFile(device, L"database/data/Error.dds", nullptr, &newDisplayObject.m_texture_diffuse);
		}

		// Apply new texture to models effect
		newDisplayObject.m_model->UpdateEffects([&](IEffect* effect)
		{
			auto lights = dynamic_cast<BasicEffect*>(effect);

			if (lights)
			{
				lights->SetTexture(newDisplayObject.m_texture_diffuse);
			}
		});

		// Position
		newDisplayObject.m_position.x = 0;
		newDisplayObject.m_position.y = 0;
		newDisplayObject.m_position.z = 0;

		// Orientation 
		newDisplayObject.m_orientation.x = 0;
		newDisplayObject.m_orientation.y = 0;
		newDisplayObject.m_orientation.z = 0;

		// Scale 
		newDisplayObject.m_scale.x = 5;
		newDisplayObject.m_scale.y = 5;
		newDisplayObject.m_scale.z = 5;

		// Wireframe/Render flags
		newDisplayObject.m_render = 1;
		newDisplayObject.m_wireframe = 0;

		newDisplayObject.m_light_type = 1;
		newDisplayObject.m_light_diffuse_r = 2;
		newDisplayObject.m_light_diffuse_g = 3;
		newDisplayObject.m_light_diffuse_b = 4;
		newDisplayObject.m_light_specular_r = 5;
		newDisplayObject.m_light_specular_g = 6;
		newDisplayObject.m_light_specular_b = 7;
		newDisplayObject.m_light_spot_cutoff = 8;
		newDisplayObject.m_light_constant = 9;
		newDisplayObject.m_light_linear = 0;
		newDisplayObject.m_light_quadratic = 1;

		m_displayList.push_back(newDisplayObject); 
	}
}

// Helper method to clear the back buffers.
void ObjectRender::Clear()
{
	m_deviceResources->PIXBeginEvent(L"Clear");

	// Clear the views.
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto renderTarget = m_deviceResources->GetBackBufferRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	// Set the viewport.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	m_deviceResources->PIXEndEvent();
}

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void ObjectRender::CreateDeviceDependentResources()
{
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto device = m_deviceResources->GetD3DDevice();

	m_states = std::make_unique<CommonStates>(device);

	m_fxFactory = std::make_unique<EffectFactory>(device);
	m_fxFactory->SetDirectory(L"database/data/"); //fx Factory will look in the database directory
	m_fxFactory->SetSharing(false);	//we must set this to false otherwise it will share effects based on the initial tex loaded (When the model loads) rather than what we will change them to.

	m_sprites = std::make_unique<SpriteBatch>(context);

	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

	m_batchEffect = std::make_unique<BasicEffect>(device);
	m_batchEffect->SetVertexColorEnabled(true);

	{
		void const* shaderByteCode;
		size_t byteCodeLength;

		m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

		DX::ThrowIfFailed(
			device->CreateInputLayout(VertexPositionColor::InputElements,
				VertexPositionColor::InputElementCount,
				shaderByteCode, byteCodeLength,
				m_batchInputLayout.ReleaseAndGetAddressOf())
		);
	}

	m_font = std::make_unique<SpriteFont>(device, L"SegoeUI_18.spritefont");

	//    m_shape = GeometricPrimitive::CreateTeapot(context, 4.f, 8);

		// SDKMESH has to use clockwise winding with right-handed coordinates, so textures are flipped in U
	m_model = Model::CreateFromSDKMESH(device, L"tiny.sdkmesh", *m_fxFactory);


	// Load textures
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"seafloor.dds", nullptr, m_texture1.ReleaseAndGetAddressOf())
	);

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"windowslogo.dds", nullptr, m_texture2.ReleaseAndGetAddressOf())
	);

}

// Allocate all memory resources that change on a window SizeChanged event.
void ObjectRender::CreateWindowSizeDependentResources()
{
	auto size = m_deviceResources->GetOutputSize();
	float aspectRatio = float(size.right) / float(size.bottom);
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	m_projection = Matrix::CreatePerspectiveFieldOfView(
		fovAngleY,
		aspectRatio,
		0.01f,
		1000.0f
	);

	m_batchEffect->SetProjection(m_projection);
}

void ObjectRender::OnDeviceLost()
{
	m_states.reset();
	m_fxFactory.reset();
	m_sprites.reset();
	m_batch.reset();
	m_batchEffect.reset();
	m_font.reset();
	m_shape.reset();
	m_model.reset();
	m_texture1.Reset();
	m_texture2.Reset();
	m_batchInputLayout.Reset();
}

void ObjectRender::OnDeviceRestored()
{
	CreateDeviceDependentResources();

	CreateWindowSizeDependentResources();
}
#pragma endregion

#pragma region Message Handlers
void ObjectRender::OnActivated()
{
	opened = true; 
}

void ObjectRender::OnDeactivated()
{
	opened = false;
}
#pragma endregion
