#include "IndexBuffer.hpp"
#include "Log.hpp"

#include <glad/glad.h>

constexpr GLenum usage_to_GLenum(const VertexBuffer::EUsage usage) {
	switch (usage) {
	case VertexBuffer::EUsage::Static: return GL_STATIC_DRAW;
	case VertexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
	case VertexBuffer::EUsage::Stream: return GL_STREAM_DRAW;
	}
	LOG_ERROR("Unknown VertexBuffer usage");
	return GL_STREAM_DRAW;
}

IndexBuffer::IndexBuffer(const void* data, const size_t count, const VertexBuffer::EUsage usage) : count(count) {
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, usage_to_GLenum(usage));
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &id);
}

void IndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}