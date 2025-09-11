#include "RoseV2/Graphics/Renderer.hpp"

#include "RoseV2/Graphics/RenderCommand.hpp"

namespace RoseV2::Graphics {

std::vector<Renderer::DrawCall> Renderer::s_DrawQueue;

void Renderer::init() {
    RenderCommand::init();
}

void Renderer::beginScene() {
    RenderCommand::clear();
    RenderCommand::setClearColor(0.2, 0.2, 0.2);
}

void Renderer::submitVertexArray(const std::shared_ptr<VertexArray> &vertexArray, const std::shared_ptr<Shader> &shader) {
    s_DrawQueue.push_back({vertexArray, shader});
}

void Renderer::endScene() {
    for (DrawCall &call : s_DrawQueue) {
        call.Shader->bind();
        call.VertexArray->bind();
        RenderCommand::drawIndexed(call.VertexArray);
    }
}

}
