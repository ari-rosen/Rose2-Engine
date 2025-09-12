#pragma once

namespace RoseV2::Graphics {

class UniformBuffer {
public:
    virtual ~UniformBuffer() = default;

    static std::shared_ptr<UniformBuffer> create(uint32_t size, uint32_t binding);

    virtual void setData(const void *data, uint32_t size, uint32_t offset = 0) = 0;
};

}
