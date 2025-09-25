#pragma once

#include <glfw/glfw3.h>

#include <glm/glm.hpp>

class Camera
{
public:
  enum class CameraMovement
  {
    None = 0,
    Forward,
    Backward,
    Left,
    Right,
  };

public:
  explicit Camera(const glm::vec3& Position = glm::vec3(0.0f, 0.0f, 0.0f), float FOV = 90.0f, float Speed = 0.5f, float MouseSensitivity = 0.1f, float Yaw = -90.0f, float Pitch = 0.0f);

  void OnUpdate(float DeltaTime);

  [[nodiscard]] const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

  void SetFOV(const float FOV) { m_FOV = FOV; }
  void SetCameraSpeed(const float Speed) { m_Speed = Speed; }
  void SetCameraZoom(const float Zoom) { m_Zoom = Zoom; }
  void SetSensitivity(const float Sensitivity) { m_Sensitivity = Sensitivity; }

  void SetPosition(const glm::vec3& Position) { m_Position = Position; }
  void SetPitch(const float Pitch) { m_Pitch = Pitch; }
  void SetYaw(const float Yaw) { m_Yaw = Yaw; }
  void SetRoll(const float Roll) { m_Roll = Roll; }

  [[nodiscard]] float GetFOV() const { return m_FOV; }
  [[nodiscard]] float GetCameraSpeed() const { return m_Speed; }
  [[nodiscard]] float GetCameraZoom() const { return m_Zoom; }
  [[nodiscard]] float GetSensitivity() const { return m_Sensitivity; }

  [[nodiscard]] glm::vec3 GetPosition() const { return m_Position; }
  [[nodiscard]] float GetPitch() const { return m_Pitch; }
  [[nodiscard]] float GetYaw() const { return m_Yaw; }
  [[nodiscard]] float GetRoll() const { return m_Roll; }


  void ProcessMovement(CameraMovement Direction, float DeltaTime);
  void ProcessMouseMovement(float XPos, float YPos, GLboolean ConstrainPitch = true);
  void ProcessMouseScroll(float YOffset);

private:
  bool m_FirstMouse = true;

  glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

  glm::vec3 m_WorldUp = { 0.0f, 1.0f, 0.0f };

  glm::vec3 m_Up = { 0.0f, 0.0f, 1.0f };
  glm::vec3 m_Right = { 1.0f, 0.0f, 0.0f };
  glm::vec3 m_Front = { 0.0f, 0.0f, -1.0f };

  //glm::vec3 m_Direction;
  //glm::vec3 m_Target;

  float m_Pitch;
  float m_Yaw;
  float m_Roll = 0.0f; // Not Used

  float m_LastMouseXPos = 0.0f;
  float m_LastMouseYPos = 0.0f;

  float m_FOV = 90.0f;
  float m_Speed = 0.5f;
  float m_Sensitivity = 0.01f;
  float m_Zoom = 45.0f;

  glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

private:
  void UpdateCameraVectors();
};
