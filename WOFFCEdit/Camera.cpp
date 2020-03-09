#include "Camera.h"

using namespace DirectX; 
using namespace DirectX::SimpleMath; 

Camera::Camera()
{
	Init(); 
}

void Camera::Init()
{
	m_moveSpeed = 0.30f;
	m_camRotRate = 3.0f;

	m_camPosition.x = 0.0f; 
	m_camPosition.y = 3.7f; 
	m_camPosition.z = -3.5f; 

	m_camOrientation.x = 0.0f;
	m_camOrientation.y = 0.0f; 
	m_camOrientation.z = 0.0f; 

	m_camLookAt.x = 0.0f; 
	m_camLookAt.y = 0.0f; 
	m_camLookAt.z = 0.0f; 

	m_camLookDirection.x = 0.0f; 
	m_camLookDirection.y = 0.0f;
	m_camLookDirection.z = 0.0f; 

	m_camRight.x = 0.0f; 
	m_camRight.y = 0.0f; 
	m_camRight.z = 0.0f;
}

Camera::~Camera()
{
}

void Camera::Update(InputCommands _input)
{
	m_InputCommands = _input; 

	Vector3 planarMotionVector = m_camLookDirection;
	planarMotionVector.y = 0.0f;

	m_camOrientation.x = m_InputCommands.mouseY; 
	m_camOrientation.y = -m_InputCommands.mouseX;

	// Create look direction from euler angles in m_camOrientation 
	m_camLookDirection.x = sin((m_camOrientation.y) * 3.1415 / 180) * sin((m_camOrientation.x) * 3.1415 / 180);
	m_camLookDirection.y = cos((m_camOrientation.x) * 3.1415 / 180); 
	m_camLookDirection.z = cos((m_camOrientation.y) * 3.1415 / 180) * sin((m_camOrientation.x) * 3.1415 / 180); 

	m_camLookDirection.Normalize();

	// Create right vector from look direction 
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);

	// Process input and update
	if (m_InputCommands.forward)
	{
		m_camPosition += m_camLookDirection * m_moveSpeed; 
	}
	if (m_InputCommands.back)
	{
		m_camPosition -= m_camLookDirection * m_moveSpeed; 
	}
	if (m_InputCommands.right)
	{
		m_camPosition += m_camRight * m_moveSpeed; 
	}
	if (m_InputCommands.left)
	{
		m_camPosition -= m_camRight * m_moveSpeed; 
	}

	// Update lookat point 
	m_camLookAt = m_camPosition + m_camLookDirection;

	// Apply camera vectors 
	//m_view = Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY); 
}

Matrix Camera::GetViewMatrix()
{
	return Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY); 
}
