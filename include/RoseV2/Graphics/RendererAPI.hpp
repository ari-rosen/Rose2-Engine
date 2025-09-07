#pragma once

namespace RoseV2::Graphics {

class RendererAPI {
public:
    enum class API { None = 0, OpenGL };
public:
    virtual ~RendererAPI() = default;

    static std::unique_ptr<RendererAPI> create();

    virtual void init() = 0;
    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void setClearColor(float r, float g, float b) = 0;
    virtual void clear() = 0;

protected:
    static API s_API;
};

} // namespace RoseV2::Graphics
