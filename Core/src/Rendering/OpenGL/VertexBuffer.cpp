#include "VertexBuffer.hpp"
#include "Log.hpp"

#include <glad/glad.h>

constexpr GLenum UsageToGLenum(const VertexBuffer::EUsage usage) {
	switch (usage)
	{
	case VertexBuffer::EUsage::Static: return GL_STATIC_DRAW;
	case VertexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
	case VertexBuffer::EUsage::Stream: return GL_STREAM_DRAW;
	}
	LOG_ERROR("Unknown VertexBuffer usage");
	return GL_STREAM_DRAW;
}

VertexBuffer::VertexBuffer(const void* data, const size_t size, const EUsage usage) {
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