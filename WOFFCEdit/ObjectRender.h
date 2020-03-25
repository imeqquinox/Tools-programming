#pragma once
#include "DeviceResources.h"
#include "StepTimer.h"
#include "SceneObject.h"
#include "DisplayObject.h"
#include "ModelInfo.h"
#include <vector>

class ObjectRender : public DX::IDeviceNotify
{
public: 
	ObjectRender(); 
	~ObjectRender(); 

	void Initialize(HWND window, int width, int height); 
	
	void Tick(int objectIndex); 
	void Render();
	void InitModels(std::vector<ModelInfo>* models); 
	void UpdateParameters(ModelInfo model); 

	void Clear(); 

	// IDeviceNotify
	virtual void OnDeviceLost() override; 
	virtual void OnDeviceRestored() override;

	// Messages
	void OnActivated(); 
	void OnDeactivated(); 

private: 
	bool opened; 
	void Update();

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();

	std::vector<DisplayObject> m_displayList; 
	int object_index; 

	// camera 
	float m_movespeed; 
	DirectX::SimpleMath::Vector3 m_camPosition; 
	DirectX::SimpleMath::Vector3 m_camOrientation; 
	DirectX::SimpleMath::Vector3 m_camLookAt; 
	DirectX::SimpleMath::Vector3 m_camLookDirection; 
	DirectX::SimpleMath::Vector3 m_camRight; 
	float m_camRotRate;

	// Device resources
	std::shared_ptr<DX::DeviceResources> m_deviceResources;

	// DirectX objects
	std::unique_ptr<DirectX::CommonStates> m_states; 
	std::unique_ptr<DirectX::BasicEffect> m_batchEffect;
	std::unique_ptr<DirectX::EffectFactory> m_fxFactory;
	std::unique_ptr<DirectX::GeometricPrimitive> m_shape; 
	std::unique_ptr<DirectX::Model> m_model; 
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch; 
	std::unique_ptr<DirectX::SpriteBatch> m_sprites;
	std::unique_ptr<DirectX::SpriteFont> m_font;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_texture1;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_texture2;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>                               m_batchInputLayout;

	DX::StepTimer m_timer; 

	DirectX::SimpleMath::Matrix m_world; 
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection; 
};

//std::wstring StringToWCHART(std::string s);

