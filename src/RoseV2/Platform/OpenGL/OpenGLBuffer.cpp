#include "RoseV2/Platform/OpenGL/OpenGLBuffer.hpp"
#include "RoseV2/Platform/OpenGL/OpenGLError.hpp"
#include <glad/glad.h>

namespace RoseV2::Platform::OpenGL {

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
    GL_CALL(glGenBuffers(1, &m_VboID));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VboID));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* data, uint32_t size) {
    GL_CALL(glGenBuffers(1, &m_VboID));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VboID));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    GL_CALL(glDeleteBuffers(1, &m_VboID));
}

void OpenGLVertexBuffer::bind() const {
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VboID));
}

void OpenGLVertexBuffer::unbind() const {
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void OpenGLVertexBuffer::setData(const void* data, uint32_t size) {
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VboID));
    GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) 
    :m_Count(count)
{
    GL_CALL(glGenBuffers(1, &m_IboID));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IboID));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    GL_CALL(glDeleteBuffers(1, &m_IboID));
}

void OpenGLIndexBuffer::bind() const {
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IboID));
}

void OpenGLIndexBuffer::unbind() const {
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

}
