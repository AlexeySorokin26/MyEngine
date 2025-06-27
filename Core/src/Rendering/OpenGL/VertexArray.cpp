#include "VertexArray.hpp"

#include <glad/glad.h>

// Create VAO->activate it->activate interesting pos and relevant vbo->link data
VertexArray::VertexArray() {
	// Create VAO
	glGenVertexArrays(1, &id);
}
VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &id);
}

void VertexArray::AddVertexBuffer(const VertexBuffer& vb) {
	// Activate buffers vao and vbo
	Bind();
	vb.Bind();

	for (const BufferElement& curEl : vb.GetLayout().GetElements()) {
		// Activate pos (by default they switchted off)
		glEnableVertexAttribArray(elementsCount);
		// Link data		
		glVertexAttribPointer(
			elementsCount,							    // pos in layout->layout(location=0) in vec3 v_pos;
			static_cast<GLint>(curEl.componentsCount),  // how many elements in data
			curEl.componentType,					    // type floar or int
			GL_FALSE,								    // norm
			static_cast<GLsizei>(vb.GetLayout().GetStride()), // stride
			reinterpret_cast<const void*>(curEl.offset) // pointer, here is offset from beggining 
		);
		++elementsCount;
	}
}

void VertexArray::SetIndexBuffer(const IndexBuffer& ib) {
	Bind();
	ib.Bind();
	indicesCount = ib.GetCount();
}

size_t VertexArray::GetIndicesCount() const {
	return indicesCount;
}

void VertexArray::Bind() const {
	// Make it current
	glBindVertexArray(id);
}
void VertexArray::Unbind() {
	glBindVertexArray(0);
}