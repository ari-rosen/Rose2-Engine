#include "RoseV2/Graphics/Renderer.hpp"
#include "RoseV2/Core/Logger.hpp"
#include "RoseV2/Graphics/Buffer.hpp"
#include "RoseV2/Graphics/RenderCommand.hpp"

namespace RoseV2::Graphics {

void Renderer::init() {
    RenderCommand::init();
    
    Core::Logger::log(Core::LogLevel::Info, "Creating Core Shader: PlainColor");
    s_CoreShaders["PlainColor"] = Shader::create(
        "src/RoseV2/Graphics/CoreShaders/PlainColor.vert",
        "src/RoseV2/Graphics/CoreShaders/PlainColor.frag"
    );
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
