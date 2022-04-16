#include "Camera.h"

Camera::Camera(vec3 position, vec3 target, vec3 worldup)
{
	Position = position;
	Forward = normalize(target - position);
	WorldUp = worldup;
	Right = normalize(cross(Forward, WorldUp));
	Up = normalize(cross(Right, Forward));
}

Camera::Camera(vec3 position, float pitch, float yaw, vec3 worldup)
{
	Position = position;
	WorldUp = worldup;
	Pitch = pitch;
	Yaw = yaw;
	Forward.x = cos(Pitch) * sin(Yaw);
	Forward.y = sin(Pitch);
	Forward.z = cos(Pitch) * cos(Yaw);
	Right = normalize(cross(Forward, WorldUp));
	Up = normalize(cross(Right, Forward));
}

mat4 Camera::GetViewMatrix()
{
	return lookAt(Position, Position + Forward, WorldUp);
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY)
{
	Pitch += deltaY * sensitivityY;
	Yaw += deltaX * sensitivityX;
	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;
	UpdateCameraVectors();
}

void Camera::UpdateCameraPosition()
{
	Position += Forward * -speedZ * sensitivityMove;
	Position += Right * speedX * sensitivityMove;
	Position += Up * speedY * sensitivityMove;
}

void Camera::UpdateCameraVectors()
{
	vec3 front;
	front.x = cos(Pitch) * sin(Yaw);
	front.y = sin(Pitch);
	front.z = cos(Pitch) * cos(Yaw);
	Forward = normalize(front);
	Right = normalize(cross(Forward, WorldUp));
	Up = normalize(cross(Right, Forward));
}