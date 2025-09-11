#pragma once

#include "RoseV2/Graphics/Buffer.hpp"
#include "RoseV2/Graphics/VertexArray.hpp"

namespace RoseV2::Platform::OpenGL {

class OpenGLVertexArray : public Graphics::VertexArray {
public:
    OpenGLVertexArray();
    ~OpenGLVertexArray() override;
    
    void addVertexBuffer(const std::shared_ptr<Graphics::VertexBuffer> &vertexBuffer) override;
    void setIndexBuffer(const std::shared_ptr<Graphics::IndexBuffer> &indexBuffer) override;

    void bind() const override;
    void unbind() const override;

    const std::vector<std::shared_ptr<Graphics::VertexBuffer>>& getVertexBuffers() const override { return m_VertexBuffers; }
    const std::shared_ptr<Graphics::IndexBuffer> &getIndexBuffer() const override { return m_IndexBuffer; }
private:
    uint32_t m_VAO;
    std::vector<std::shared_ptr<Graphics::VertexBuffer>> m_VertexBuffers;
    std::shared_ptr<Graphics::IndexBuffer> m_IndexBuffer;
};

}
