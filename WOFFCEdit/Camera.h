#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include "InputCommands.h" 

class Camera
{
public:
	Camera();
	~Camera();
	void Init(); 
	void Update(InputCommands _input); 
	DirectX::SimpleMath::Matrix GetViewMatrix(); 

private:
	InputCommands m_InputCommands; 

	DirectX::SimpleMath::Vector3 m_camPosition;
	DirectX::SimpleMath::Vector3 m_camOrientation; 
	DirectX::SimpleMath::Vector3 m_camLookAt; 
	DirectX::SimpleMath::Vector3 m_camLookDirection; 
	DirectX::SimpleMath::Vector3 m_camRight;
	float m_camRotRate; 
	float m_moveSpeed; 

	DirectX::SimpleMath::Matrix m_world; 
	DirectX::SimpleMath::Matrix m_view; 
	DirectX::SimpleMath::Matrix m_projection; 
};

