#pragma once

#include "RoseV2/Graphics/RendererAPI.hpp"

namespace RoseV2::Graphics {

class RenderCommand {
public:
    static void init();
    static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    static void setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    static void setClearColor(float r, float g, float b);
    static void clear(); 

    static void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray);


private:
    static std::unique_ptr<RendererAPI> s_RendererAPI;

};

}
