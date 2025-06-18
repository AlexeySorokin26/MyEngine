#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

class ShaderProgram {
public:
	ShaderProgram(const char* vertex_shader, const char* frag_shader);
	~ShaderProgram();
	bool IsCompiled() { return isCompiled; }
	void Bind();
	void UnBind();
	void SetMatrix4(const char* name, const glm::mat4 m) const;
private:
	bool isCompiled = false;
	unsigned int id;
	bool CreateShader(const char* source, const GLenum shaderType, GLuint& shaderId);
};