#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace glm;

class Camera
{
public:
	Camera(vec3 position, vec3 target, vec3 worldup);
	Camera(vec3 position, float pitch, float yaw, vec3 worldup);

	vec3 Position;
	vec3 Forward;
	vec3 Right;
	vec3 Up;
	vec3 WorldUp;
	float Pitch;
	float Yaw;
	float speedZ = 0.0f;
	float speedX = 0.0f;
	float speedY = 0.0f;
	float sensitivityX = 0.002f;
	float sensitivityY = 0.002f;
	float sensitivityMove = 0.05f;

	mat4 GetViewMatrix();
	void ProcessMouseMovement(float deltaX, float deltaY);
	void UpdateCameraPosition();

private:
	void UpdateCameraVectors();
};

