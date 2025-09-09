#pragma once

#include "RoseV2/Graphics/Buffer.hpp"
namespace RoseV2::Platform::OpenGL {

class OpenGLVertexBuffer : public Graphics::VertexBuffer {
public:
    OpenGLVertexBuffer(uint32_t size);
    OpenGLVertexBuffer(float* data, uint32_t size);
    ~OpenGLVertexBuffer() override;

    void bind() const override;
    void unbind() const override;

    void setData(const void* data, uint32_t size) override;

    const Graphics::BufferLayout& getBufferLayout() override { return m_Layout; }
    void setBufferLayout(const Graphics::BufferLayout& layout) override { m_Layout = layout; }
private:
    uint32_t m_VboID;
    Graphics::BufferLayout m_Layout;
};

class OpenGLIndexBuffer : public Graphics::IndexBuffer {
public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
    ~OpenGLIndexBuffer() override;

    void bind() const override;
    void unbind() const override;

    uint32_t getCount() override { return m_Count; }
private:
    uint32_t m_IboID;
    uint32_t m_Count;
};

}
