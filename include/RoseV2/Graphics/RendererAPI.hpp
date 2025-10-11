#pragma once

#include "RoseV2/Graphics/VertexArray.hpp"
namespace RoseV2::Graphics {

class RendererAPI {
public:
    enum class API { None = 0, OpenGL = 1 };
public:
    virtual ~RendererAPI() = default;

    static std::unique_ptr<RendererAPI> create();

    inline static API getAPI() { return s_API; }

    virtual void init() = 0;
    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void setClearColor(float r, float g, float b) = 0;
    virtual void clear() = 0;

    virtual void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray, uint32_t indexCount) = 0;

protected:
    static API s_API;
};

} // namespace RoseV2::Graphics
