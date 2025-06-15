#include "ShaderProgram.hpp"
#include "Log.hpp"

#include <string>

ShaderProgram::ShaderProgram(const char* vertex_shader, const char* frag_shader) {

	GLuint vs = 0;
	if (!CreateShader(vertex_shader, GL_VERTEX_SHADER, vs)) {
		LOG_CRIT("Vertex shader: compile-time error!");
		glDeleteShader(vs);
		return;
	}

	GLuint fs = 0;
	if (!CreateShader(frag_shader, GL_FRAGMENT_SHADER, fs)) {
		LOG_CRIT("Fragment shader: compile-time error!");
		glDeleteShader(vs);
		glDeleteShader(fs);
		return;
	}

	// Create prog and link shaders
	id = glCreateProgram();
	glAttachShader(id, vs);
	glAttachShader(id, fs);
	glLinkProgram(id);
	GLint success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		char infoLog[1024];
		glGetShaderInfoLog(id, 1024, nullptr, infoLog);
		std::string a(infoLog);
		LOG_CRIT("Shader link error:\n{}", infoLog);
		glDeleteProgram(id);
		id = 0;
		glDeleteShader(vs);
		glDeleteShader(fs);

		return;
	}
	else {
		isCompiled = true;
	}
	// cleanup
	// Detach them first 
	glDetachShader(id, vs);
	glDetachShader(id, fs);
	// Now delete
	glDeleteShader(vs);
	glDeleteShader(fs);
}

bool ShaderProgram::CreateShader(const char* source, const GLenum shaderType, GLuint& shaderId) {
	// Create->Pass data->Comple
	shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &source, nullptr); // 3 arg in a pointer to array (pointer to pointer)
	glCompileShader(shaderId);

	GLint success;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		char infoLog[1024];
		glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);

		std::string a(infoLog);
		LOG_CRIT("Shader compilation error:\n{}", infoLog);
		return false;
	}
	return true;
}

void ShaderProgram::Bind() {
	glUseProgram(id);
}

void ShaderProgram::Bind() {
	glUseProgram(0);
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(id);
}