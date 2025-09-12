#include "RoseV2/Platform/OpenGL/OpenGLUniformBuffer.hpp"

#include <glad/glad.h>

namespace RoseV2::Platform::OpenGL {

OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding) {
    glGenBuffers(1, &m_UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);

    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_UBO);
}

OpenGLUniformBuffer::~OpenGLUniformBuffer() {
    glDeleteBuffers(1, &m_UBO);
}

void OpenGLUniformBuffer::setData(const void *data, uint32_t size, uint32_t offset) {
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

}
