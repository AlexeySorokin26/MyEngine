#pragma once 

#include "VertexBuffer.hpp"

class IndexBuffer {
public:
	IndexBuffer(const void* data, const size_t count, const VertexBuffer::EUsage usage = VertexBuffer::EUsage::Static);
	~IndexBuffer();

	void Bind() const;
	void Unbind();
	size_t GetCount() const { return count; }

private:
	unsigned int id = 0;
	size_t count;
};