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

void ObjectRender::Tick()
{
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
	int numRenderObjects = m_displayList.size();
	for (int i = 0; i < numRenderObjects; i++)
	{
		m_deviceResources->PIXBeginEvent(L"Draw model");
		const XMVECTORF32 scale = { m_displayList[i].m_scale.x, m_displayList[i].m_scale.y, m_displayList[i].m_scale.z };
		const XMVECTORF32 translate = { m_displayList[i].m_position.x, m_displayList[i].m_position.y, m_displayList[i].m_position.z };

		//convert degrees into radians for rotation matrix
		XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(m_displayList[i].m_orientation.y *3.1415 / 180,
			m_displayList[i].m_orientation.x *3.1415 / 180,
			m_displayList[i].m_orientation.z *3.1415 / 180);

		XMMATRIX local = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);

		m_displayList[i].m_model->Draw(context, *m_states, local, m_view, m_projection, false);	//last variable in draw,  make TRUE for wireframe

		m_deviceResources->PIXEndEvent();
	}
	m_deviceResources->PIXEndEvent();

	m_deviceResources->Present();
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

void ObjectRender::BuildDisplayList(std::vector<SceneObject> * SceneGraph)
{
	auto device = m_deviceResources->GetD3DDevice();
	auto devicecontext = m_deviceResources->GetD3DDeviceContext();

	if (!m_displayList.empty())		//is the vector empty
	{
		m_displayList.clear();		//if not, empty it
	}

	//for every item in the scenegraph
	int numObjects = SceneGraph->size();
	for (int i = 0; i < numObjects; i++)
	{

		//create a temp display object that we will populate then append to the display list.
		DisplayObject newDisplayObject;

		//load model
		std::wstring modelwstr = StringToWCHART(SceneGraph->at(i).model_path);							//convect string to Wchar
		newDisplayObject.m_model = Model::CreateFromCMO(device, modelwstr.c_str(), *m_fxFactory, true);	//get DXSDK to load model "False" for LH coordinate system (maya)

		//Load Texture
		std::wstring texturewstr = StringToWCHART(SceneGraph->at(i).tex_diffuse_path);								//convect string to Wchar
		HRESULT rs;
		rs = CreateDDSTextureFromFile(device, texturewstr.c_str(), nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource

		//if texture fails.  load error default
		if (rs)
		{
			CreateDDSTextureFromFile(device, L"database/data/Error.dds", nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource
		}

		//apply new texture to models effect
		newDisplayObject.m_model->UpdateEffects([&](IEffect* effect) //This uses a Lambda function,  if you dont understand it: Look it up.
		{
			auto lights = dynamic_cast<BasicEffect*>(effect);
			if (lights)
			{
				lights->SetTexture(newDisplayObject.m_texture_diffuse);
			}
		});

		//set position
		newDisplayObject.m_position.x = SceneGraph->at(i).posX;
		newDisplayObject.m_position.y = SceneGraph->at(i).posY;
		newDisplayObject.m_position.z = SceneGraph->at(i).posZ;

		//setorientation
		newDisplayObject.m_orientation.x = SceneGraph->at(i).rotX;
		newDisplayObject.m_orientation.y = SceneGraph->at(i).rotY;
		newDisplayObject.m_orientation.z = SceneGraph->at(i).rotZ;

		//set scale
		newDisplayObject.m_scale.x = SceneGraph->at(i).scaX;
		newDisplayObject.m_scale.y = SceneGraph->at(i).scaY;
		newDisplayObject.m_scale.z = SceneGraph->at(i).scaZ;

		//set wireframe / render flags
		newDisplayObject.m_render = SceneGraph->at(i).editor_render;
		newDisplayObject.m_wireframe = SceneGraph->at(i).editor_wireframe;

		newDisplayObject.m_light_type = SceneGraph->at(i).light_type;
		newDisplayObject.m_light_diffuse_r = SceneGraph->at(i).light_diffuse_r;
		newDisplayObject.m_light_diffuse_g = SceneGraph->at(i).light_diffuse_g;
		newDisplayObject.m_light_diffuse_b = SceneGraph->at(i).light_diffuse_b;
		newDisplayObject.m_light_specular_r = SceneGraph->at(i).light_specular_r;
		newDisplayObject.m_light_specular_g = SceneGraph->at(i).light_specular_g;
		newDisplayObject.m_light_specular_b = SceneGraph->at(i).light_specular_b;
		newDisplayObject.m_light_spot_cutoff = SceneGraph->at(i).light_spot_cutoff;
		newDisplayObject.m_light_constant = SceneGraph->at(i).light_constant;
		newDisplayObject.m_light_linear = SceneGraph->at(i).light_linear;
		newDisplayObject.m_light_quadratic = SceneGraph->at(i).light_quadratic;

		m_displayList.push_back(newDisplayObject);
	}
}

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
