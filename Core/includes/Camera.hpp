#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

class Camera {
public:
	enum class ProjectionMode {
		Perspective,
		Orthographic
	};

	Camera(const glm::vec3& pos = { 0,0,0 }, const glm::vec3& rotation = { 0,0,0 },
		const ProjectionMode projMode = ProjectionMode::Perspective);

	void SetPos(const glm::vec3& p);
	void SetRotation(const glm::vec3& r);
	void SetPosRot(const glm::vec3& p, const glm::vec3& r);
	void SetProjMode(const ProjectionMode projMode);
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjMatrix() const;

	const glm::vec3& GetCamPos() const { return pos; }
	const glm::vec3& GetCamRotation() const { return rotation; }

	void MoveForward(const float delta);
	void MoveRight(const float delta);
	void MoveUp(const float delta);
	void AddMovementAndRotation(const glm::vec3& movementDelta, const glm::vec3 rotationDelta);

private:
	ProjectionMode projMode;
	glm::vec3 pos;
	glm::vec3 rotation;

	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;

	static constexpr glm::vec3 worldUp{ 0.f, 0.f, 1.f };
	static constexpr glm::vec3 worldRight{ 0.f, -1.f, 0.f };
	static constexpr glm::vec3 worldForward{ 1.f, 0.f, 0.f };

	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;
};