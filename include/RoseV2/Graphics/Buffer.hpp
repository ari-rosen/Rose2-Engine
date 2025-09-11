#pragma once

#include "RoseV2/Core/Core.hpp"
namespace RoseV2::Graphics {

enum class ShaderDataType {
    Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

struct ROSE_V2_API BufferElement {
    std::string    Name;
    ShaderDataType Type;
    uint32_t       Size;
    uint32_t       Offset;
    uint32_t       Location;
    bool           Normalized;

    BufferElement() = default;
    BufferElement(ShaderDataType type, const std::string &name, bool normalized = false);
};

struct ROSE_V2_API BufferLayout {
    std::vector<BufferElement> Elements;
    uint32_t    Stride;

    BufferLayout() = default;
    BufferLayout(const std::initializer_list<BufferElement>& elements);
};

class ROSE_V2_API VertexBuffer {
public:
    virtual ~VertexBuffer() = default;

    static std::shared_ptr<VertexBuffer> create(uint32_t size);
    static std::shared_ptr<VertexBuffer> create(float *vertices, uint32_t size);
   
    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void setData(const void *data, uint32_t size) = 0;

    virtual const BufferLayout& getBufferLayout() = 0;
    virtual void setBufferLayout(const BufferLayout &layout) = 0;
};

class ROSE_V2_API IndexBuffer {
public:
    virtual ~IndexBuffer() = default;

    static std::shared_ptr<IndexBuffer> create(uint32_t *indices, uint32_t count);

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual uint32_t getCount() = 0;
};

}
