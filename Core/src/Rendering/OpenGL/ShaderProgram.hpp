#pragma once

#include <glad/glad.h>

class ShaderProgram {
public:
	ShaderProgram(const char* vertex_shader, const char* frag_shader);
	~ShaderProgram();
	bool IsCompiled() { return isCompiled; }
	void Bind();
	void UnBind();
private:
	bool isCompiled = false;
	unsigned int id;
	bool CreateShader(const char* source, const GLenum shaderType, GLuint& shaderId);
};