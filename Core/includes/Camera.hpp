
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

private:
	ProjectionMode projMode;
	glm::vec3 pos;
	glm::vec3 rotation;
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;
};