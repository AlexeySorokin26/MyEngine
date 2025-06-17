#include "VertexBuffer.hpp"
#include "Log.hpp"

#include <glad/glad.h>

/// Get how many components has this type
constexpr unsigned int ShaderDataTypeToComponentsCount(const ShaderDataType type) {
	switch (type) {
	case ShaderDataType::Float:
	case ShaderDataType::Int:
		return 1;

	case ShaderDataType::Float2:
	case ShaderDataType::Int2:
		return 2;

	case ShaderDataType::Float3:
	case ShaderDataType::Int3:
		return 3;

	case ShaderDataType::Float4:
	case ShaderDataType::Int4:
		return 4;
	}
	LOG_ERROR("ShaderDataTypeToComponentsCount: Unknown ShaderDataType");
	return 0;
}

/// Get how many bytes has this type 
constexpr size_t ShaderDataTypeSize(const ShaderDataType type) {
	switch (type) {
	case ShaderDataType::Float:
	case ShaderDataType::Float2:
	case ShaderDataType::Float3:
	case ShaderDataType::Float4:
		return sizeof(GLfloat) * ShaderDataTypeToComponentsCount(type);

	case ShaderDataType::Int:
	case ShaderDataType::Int2:
	case ShaderDataType::Int3:
	case ShaderDataType::Int4:
		return sizeof(GLint) * ShaderDataTypeToComponentsCount(type);
	}
	LOG_ERROR("ShaderDataTypeSize: Unknown ShaderDataType");
	return 0;
}

/// Convert own data type to OpenGL type
constexpr unsigned int ShaderDataTypeToComponentType(const ShaderDataType type) {
	switch (type) {
	case ShaderDataType::Float:
	case ShaderDataType::Float2:
	case ShaderDataType::Float3:
	case ShaderDataType::Float4:
		return GL_FLOAT;

	case ShaderDataType::Int:
	case ShaderDataType::Int2:
	case ShaderDataType::Int3:
	case ShaderDataType::Int4:
		return GL_INT;
	}
	LOG_ERROR("ShaderDataTypeToComponentType: Unknown ShaderDataType");
	return GL_FLOAT;
}

constexpr GLenum UsageToGLenum(const VertexBuffer::EUsage usage) {
	switch (usage)
	{
	case VertexBuffer::EUsage::Static: return GL_STATIC_DRAW;
	case VertexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
	case VertexBuffer::EUsage::Stream: return GL_STREAM_DRAW;
	}
	LOG_ERROR("UsageToGLenum: Unknown VertexBuffer usage");
	return GL_STREAM_DRAW;
}

BufferElement::BufferElement(const ShaderDataType type) :
	type(type),
	componentType(ShaderDataTypeToComponentType(type)),
	componentsCount(ShaderDataTypeToComponentsCount(type)),
	size(ShaderDataTypeSize(type)),
	offset(0)
{

}

VertexBuffer::VertexBuffer(const void* data, const size_t size, BufferLayout bl, const EUsage usage) :
	bufferLayout(std::move(bl)) {
	// Pass data to gpu
	// Create vbo->Make it current->Copy data from CPU to GPU
	glGenBuffers(1, &id);
	// Link it and make it current
	Bind();
	// Copy from CPU to GPU memory
	glBufferData(GL_ARRAY_BUFFER, size, data, UsageToGLenum(usage));

}
VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &id);
}

void VertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}