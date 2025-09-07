#include "RoseV2/Graphics/Renderer.hpp"
#include "RoseV2/Graphics/RenderCommand.hpp"

namespace RoseV2::Graphics {

void Renderer::init() {
    RenderCommand::init();
}

void Renderer::beginScene() {
    RenderCommand::clear();
    RenderCommand::setClearColor(0.2, 0.2, 0.2);
}

void Renderer::submitSprite() {

}

void Renderer::endScene() {

}

}
