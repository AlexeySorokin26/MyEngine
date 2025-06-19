#include "Camera.hpp"

#include <glm/trigonometric.hpp>

Camera::Camera(const glm::vec3& pos, const glm::vec3& rotation, const ProjectionMode projMode) :
	pos(pos), rotation(rotation), projMode(projMode)
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void Camera::SetPos(const glm::vec3& p)
{
	pos = p;
	UpdateViewMatrix();
}

void Camera::SetRotation(const glm::vec3& r)
{
	rotation = r;
	UpdateViewMatrix();
}

void Camera::SetPosRot(const glm::vec3& p, const glm::vec3& r)
{
	pos = p;
	rotation = r;
	UpdateViewMatrix();
}

void Camera::SetProjMode(const ProjectionMode projMode)
{
	this->projMode = projMode;
	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix() {
	glm::mat4 translationMat(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-pos[0], -pos[1], -pos[2], 1
	);
	// Z 
	float rotateRadZ = glm::radians(-rotation.z);
	glm::mat4 rotateZMat(
		cos(rotateRadZ), sin(rotateRadZ), 0, 0,
		-sin(rotateRadZ), cos(rotateRadZ), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	// Y
	float rotateRadY = glm::radians(-rotation.y);
	glm::mat4 rotateYMat(
		cos(rotateRadY), 0, -sin(rotateRadY), 0,
		0, 1, 0, 0,
		sin(rotateRadY), 0, cos(rotateRadY), 0,
		0, 0, 0, 1
	);
	// X
	float rotateRadX = glm::radians(-rotation.x);
	glm::mat4 rotateXMat(
		1, 0, 0, 0,
		0, cos(rotateRadX), sin(rotateRadX), 0,
		0, -sin(rotateRadX), cos(rotateRadX), 0,
		0, 0, 0, 1
	);
	viewMatrix = rotateYMat * rotateXMat * translationMat;
}

void Camera::UpdateProjectionMatrix() {
	if (projMode == ProjectionMode::Perspective) {
		float r = 0.1f;
		float t = 0.1f;
		float f = 10.f;
		float n = 0.1f;
		projMatrix = glm::mat4(
			n / r, 0, 0, 0,
			0, n / t, 0, 0,
			0, 0, (-f - n) / (f - n), -1,
			0, 0, -2 * f * n / (f - n), 0
		);
	}
	else {
		float r = 2.f;
		float t = 2.f;
		float f = 100.f;
		float n = 0.1f;
		projMatrix = glm::mat4(
			1 / r, 0, 0, 0,
			0, 1 / t, 0, 0,
			0, 0, -2 / (f - n), 0,
			0, 0, (-f - n) / (f - n), 1
		);
	}
}

glm::mat4 Camera::GetViewMatrix() const
{
	return viewMatrix;
}

glm::mat4 Camera::GetProjMatrix() const
{
	return projMatrix;
}

