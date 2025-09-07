#include "RoseV2/Platform/OpenGL/OpenGLRendererAPI.hpp"

#include <glad/glad.h>

namespace RoseV2::Platform::OpenGL {

void OpenGLRendererAPI::init() {
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
}

void OpenGLRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    glViewport(x, y, width, height); 
}

void OpenGLRendererAPI::setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    glScissor(x, y, width, height);
}

void OpenGLRendererAPI::setClearColor(float r, float g, float b) {
    glClearColor(r, g, b, 1.0);
}

void OpenGLRendererAPI::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

}
