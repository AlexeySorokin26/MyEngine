#include "Camera.hpp"

#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

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

	const float rollRad = glm::radians(rotation.x);
	const float pitchRad = glm::radians(rotation.y);
	const float yawRad = glm::radians(rotation.z);

	// Z 
	glm::mat3 rotateZMat(
		cos(rollRad), sin(rollRad), 0,
		-sin(rollRad), cos(rollRad), 0,
		0, 0, 1
	);
	// Y
	glm::mat3 rotateYMat(
		cos(pitchRad), 0, -sin(pitchRad),
		0, 1, 0,
		sin(pitchRad), 0, cos(pitchRad)
	);
	// X
	glm::mat3 rotateXMat(
		1, 0, 0,
		0, cos(yawRad), sin(yawRad),
		0, -sin(yawRad), cos(yawRad)
	);

	const glm::mat3 eulerRotateMatr = rotateZMat * rotateYMat * rotateXMat;

	direction = glm::normalize(eulerRotateMatr * worldForward);
	right = glm::normalize(eulerRotateMatr * worldRight);
	up = glm::cross(right, direction);

	viewMatrix = glm::lookAt(pos, pos + direction, up);
}

void  Camera::AddMovementAndRotation(const glm::vec3& movementDelta, const glm::vec3 rotationDelta) {
	pos += direction * movementDelta.x;
	pos += right * movementDelta.y;
	pos += up * movementDelta.z;

	rotation += rotationDelta;

	UpdateViewMatrix(); 
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

void Camera::MoveForward(const float delta) {
	pos += direction * delta;
}
void Camera::MoveRight(const float delta) {
	pos += right * delta;
}
void Camera::MoveUp(const float delta) {
	pos += up * delta;
}
