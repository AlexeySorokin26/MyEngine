#pragma once

#include <vector>

// Every vertex buffer have a laout explaing how data is packed in it

enum class ShaderDataType {
	Float,
	Float2,
	Float3,
	Float4,
	Int,
	Int2,
	Int3,
	Int4,
};
// Element is Float2 with 2 Components
struct BufferElement {
	ShaderDataType type;    // Float, FLoat2, Float3, Float4
	uint32_t componentType; // Float or Int
	size_t componentsCount; // Float->1, Float2->2, Float3->3, Float4->4
	size_t size;			// How many bytes Floate3->4bytes * 3 = 12 bytes
	size_t offset;          // How many bytes from start 
	// Example
	// For pos 0 
	// For col 4bytes*3 = 12 bytes
	// GLfloat posColors[] = {
	//	0.0f, 0.5f, 0.0f,	 1.0f, 0.0f, 0.0f,
	//	0.5f, -0.5f, 0.0f,	 0.0f, 1.0f, 0.0f,
	//	-0.5f, -0.5f, 0.0f,	 0.0f, 0.0f, 1.0f,
	//};
	// For example with only 1 pos or 1 col stride is 0
	BufferElement(const ShaderDataType type);
};

class BufferLayout {
public:
	BufferLayout(std::initializer_list<BufferElement> es) : elements(std::move(es)) {
		size_t offset = 0;
		stride = 0;
		for (auto& el : elements) {
			el.offset = offset;
			offset += el.size;
			stride += el.size;
		}
	}
	const std::vector<BufferElement>& GetElements() const { return elements; }
	size_t GetStride() const { return stride; }
private:
	std::vector<BufferElement> elements;
	size_t stride = 0;
	// In how many bytes we have the next element
	// Example here we have one element pos + col. 
	// So next element in 4bytes * 6 = 24 bytes
	// GLfloat posColors[] = {
	// 0.0f, 0.5f, 0.0f,	     1.0f, 0.0f, 0.0f,
	// 0.5f, -0.5f, 0.0f,	 0.0f, 1.0f, 0.0f,
	// -0.5f, -0.5f, 0.0f,	 0.0f, 0.0f, 1.0f,
	// };
	// Another example ???
	/*GLfloat points[] = {
	 0.0f, 0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 -0.5f, -0.5f, 0.0f
	};*/
	// Here we have one element and its one point only
	// So 4bytes * 3 = 12 bytes

};

class VertexBuffer {
public:
	enum class EUsage {
		Static,
		Dynamic,
		Stream
	};

	VertexBuffer(const void* data, const size_t size, BufferLayout bl, const EUsage usage = VertexBuffer::EUsage::Static);
	~VertexBuffer();

	void Bind() const;
	void Unbind();

	const BufferLayout& GetLayout() const { return bufferLayout; }

private:
	unsigned int id = 0;
	BufferLayout bufferLayout;
};