#pragma once

#include "RoseV2/Core/Core.hpp"
#include <RoseV2/Graphics/Buffer.hpp>

namespace RoseV2::Graphics {

class ROSE_V2_API VertexArray {
public:
    virtual ~VertexArray() = default;

    static std::shared_ptr<VertexArray> create();

    virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) = 0;
    virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) = 0;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const = 0;
    virtual const std::shared_ptr<IndexBuffer> &getIndexBuffer() const = 0;
};

}
