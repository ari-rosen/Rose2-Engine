#include "RoseV2/Graphics/RenderCommand.hpp"

namespace RoseV2::Graphics {

std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI;

void RenderCommand::init() {
    s_RendererAPI = RendererAPI::create();
    s_RendererAPI->init();
}

void RenderCommand::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    s_RendererAPI->setViewport(x, y, width, height);
}

void RenderCommand::setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    s_RendererAPI->setViewport(x, y, width, height);
}

void RenderCommand::setClearColor(float r, float g, float b) {
    s_RendererAPI->setClearColor(r, r, g);
}

void RenderCommand::clear() {
    s_RendererAPI->clear();
}

}
