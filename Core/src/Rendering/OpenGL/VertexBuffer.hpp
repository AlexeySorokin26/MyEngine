#pragma once

class VertexBuffer {
public:
	enum class EUsage {
		Static,
		Dynamic,
		Stream
	};

	VertexBuffer(const void* data, const size_t size, const EUsage usage = VertexBuffer::EUsage::Static);
	~VertexBuffer();

	void Bind() const;
	void Unbind();

private:
	unsigned int id = 0;
};