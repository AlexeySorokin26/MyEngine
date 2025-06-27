#pragma once

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void AddVertexBuffer(const VertexBuffer& vb);
	void SetIndexBuffer(const IndexBuffer& ib);
	void Bind() const;
	void Unbind();
	size_t GetIndicesCount() const;
private:
	unsigned int id = 0;
	unsigned int elementsCount = 0; // amout of vbos and their elements
	size_t indicesCount = 0;
};