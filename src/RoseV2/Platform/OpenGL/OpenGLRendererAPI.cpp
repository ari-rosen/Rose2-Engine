#include "RoseV2/Platform/OpenGL/OpenGLRendererAPI.hpp"

#include "RoseV2/Platform/OpenGL/OpenGLError.hpp"

#include <glad/glad.h>

namespace RoseV2::Platform::OpenGL {

void OpenGLRendererAPI::init() {
    GL_CALL(glEnable(GL_BLEND));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glEnable(GL_LINE_SMOOTH));
}

void OpenGLRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    GL_CALL(glViewport(x, y, width, height)); 
}

void OpenGLRendererAPI::setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    GL_CALL(glScissor(x, y, width, height));
}

void OpenGLRendererAPI::setClearColor(float r, float g, float b) {
    GL_CALL(glClearColor(r, g, b, 1.0));
}

void OpenGLRendererAPI::clear() {
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void OpenGLRendererAPI::drawIndexed(const std::shared_ptr<Graphics::VertexArray> &vertexArray) {
    vertexArray->bind();

    GL_CALL(glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr));
}

}
