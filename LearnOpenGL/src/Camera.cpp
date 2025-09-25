#include "Camera.hpp"

#include "glm/ext/matrix_transform.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& Position, float FOV, float Speed, float MouseSensitivity, float Yaw, float Pitch)
  : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_Speed(Speed), m_Sensitivity(MouseSensitivity)
{
  m_Position = Position;
  m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  m_Yaw = Yaw;
  m_Pitch = Pitch;
  UpdateCameraVectors();
}

void Camera::OnUpdate(float DeltaTime)
{
  m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::ProcessMovement(const CameraMovement Direction, const float DeltaTime)
{
  const float Velocity = m_Speed * DeltaTime;
  if (Direction == CameraMovement::Forward)
    m_Position += m_Front * Velocity;
  if (Direction == CameraMovement::Backward)
    m_Position -= m_Front * Velocity;
  if (Direction == CameraMovement::Left)
    m_Position -= m_Right * Velocity;
  if (Direction == CameraMovement::Right)
    m_Position += m_Right * Velocity;
}


void Camera::ProcessMouseMovement(float XPos, float YPos, const GLboolean ConstrainPitch)
{
  if (m_FirstMouse)
  {
    m_FirstMouse = false;

    m_LastMouseXPos = XPos;
    m_LastMouseYPos = YPos;
    return;
  }

  float XOffset = m_LastMouseXPos - XPos;
  float YOffset = m_LastMouseYPos - YPos;
  m_LastMouseXPos = XPos;
  m_LastMouseYPos = YPos;

  XOffset *= m_Sensitivity;
  YOffset *= m_Sensitivity;

  m_Yaw -= XOffset;
  m_Pitch += YOffset;

  if (ConstrainPitch)
  {
    if (m_Pitch > 89.0f)
      m_Pitch = 89.0f;
    if (m_Pitch < -89.0f)
      m_Pitch = -89.0f;
  }

  UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(const float YOffset)
{
  m_Zoom -= YOffset;
  if (m_Zoom < 1.0f)
    m_Zoom = 1.0f;
  if (m_Zoom > 45.0f)
    m_Zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
  glm::vec3 Front;
  Front.x = cosf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
  Front.y = sinf(glm::radians(m_Pitch));
  Front.z = sinf(glm::radians(m_Yaw)) * cosf(glm::radians(m_Pitch));
  m_Front = glm::normalize(Front);

  m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
  m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

