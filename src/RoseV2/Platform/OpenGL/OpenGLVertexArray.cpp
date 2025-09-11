#include "RoseV2/Platform/OpenGL/OpenGLVertexArray.hpp"
#include "RoseV2/Platform/OpenGL/OpenGLError.hpp"

#include "RoseV2/Graphics/Buffer.hpp"

#include <glad/glad.h>

namespace RoseV2::Platform::OpenGL {

OpenGLVertexArray::OpenGLVertexArray() {
    GL_CALL(glGenVertexArrays(1, &m_VAO));
}

OpenGLVertexArray::~OpenGLVertexArray() {
    GL_CALL(glDeleteVertexArrays(1, &m_VAO));
}

void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<Graphics::VertexBuffer>& vertexBuffer) {
    GL_CALL(glBindVertexArray(m_VAO));
    vertexBuffer->bind();

    const auto& layout = vertexBuffer->getBufferLayout();

    for (const auto& element : layout.Elements) {
        uint32_t location = element.Location;
        uint32_t size     = element.Size;
        uint32_t offset   = element.Offset;

        switch (element.Type) {
            case Graphics::ShaderDataType::Float: 
            case Graphics::ShaderDataType::Float2: 
            case Graphics::ShaderDataType::Float3: 
            case Graphics::ShaderDataType::Float4:
                GL_CALL(glEnableVertexAttribArray(location));
                GL_CALL(glVertexAttribPointer(
                    location,
                    element.Type == Graphics::ShaderDataType::Float  ? 1 :
                    element.Type == Graphics::ShaderDataType::Float2 ? 2 :
                    element.Type == Graphics::ShaderDataType::Float3 ? 3 : 4,
                    GL_FLOAT,
                    element.Normalized ? GL_TRUE : GL_FALSE,
                    layout.Stride,
                    (const void*)(uintptr_t)offset
                ));
                break;

            case Graphics::ShaderDataType::Int:
            case Graphics::ShaderDataType::Int2:
            case Graphics::ShaderDataType::Int3:
            case Graphics::ShaderDataType::Int4:
            case Graphics::ShaderDataType::Bool:
                GL_CALL(glEnableVertexAttribArray(location));
                GL_CALL(glVertexAttribIPointer(
                    location,
                    element.Type == Graphics::ShaderDataType::Int   ? 1 :
                    element.Type == Graphics::ShaderDataType::Int2  ? 2 :
                    element.Type == Graphics::ShaderDataType::Int3  ? 3 : 4,
                    GL_INT,
                    layout.Stride,
                    (const void*)(uintptr_t)offset
                ));
                break;

            case Graphics::ShaderDataType::Mat3: {
                for (uint32_t i = 0; i < 3; i++) {
                    GL_CALL(glEnableVertexAttribArray(location + i));
                    GL_CALL(glVertexAttribPointer(
                        location + i,
                        3, // vec3
                        GL_FLOAT,
                        element.Normalized ? GL_TRUE : GL_FALSE,
                        layout.Stride,
                        (const void*)(uintptr_t)(offset + sizeof(float) * 3 * i)
                    ));
                }
                break;
            }

            case Graphics::ShaderDataType::Mat4: {
                for (uint32_t i = 0; i < 4; i++) {
                    GL_CALL(glEnableVertexAttribArray(location + i));
                    GL_CALL(glVertexAttribPointer(
                        location + i,
                        4, // vec4
                        GL_FLOAT,
                        element.Normalized ? GL_TRUE : GL_FALSE,
                        layout.Stride,
                        (const void*)(uintptr_t)(offset + sizeof(float) * 4 * i)
                    ));
                }
                break;
            }

            default:
                break;
        }
    }
}

void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<Graphics::IndexBuffer> &indexBuffer) {
    m_IndexBuffer = indexBuffer;
    glBindVertexArray(m_VAO);
    indexBuffer->bind();
}

void OpenGLVertexArray::bind() const {
    glBindVertexArray(m_VAO);
}

void OpenGLVertexArray::unbind() const {
    glBindVertexArray(0);
}

}
