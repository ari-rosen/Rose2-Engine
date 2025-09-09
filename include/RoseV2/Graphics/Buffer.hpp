#pragma once

namespace RoseV2::Graphics {

struct BufferElement {
    std::string Name;
    uint32_t    Size;
    uint32_t    Offset;
    uint32_t    Location;
};

struct BufferLayout {
    std::vector<BufferElement> Elements;
    uint32_t    Stride;
};

class VertexBuffer {
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

class IndexBuffer {
public:
    virtual ~IndexBuffer() = default;

    static std::shared_ptr<IndexBuffer> create(uint32_t *indices, uint32_t count);

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual uint32_t getCount() = 0;
};

}
