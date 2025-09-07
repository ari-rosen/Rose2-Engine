#include "RoseV2/Platform/OpenGL/OpenGLRenderer.hpp"

#include "RoseV2/Core/Assert.hpp"
#include "RoseV2/Core/Logger.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace RoseV2::Platform::OpenGL {

void OpenGLRenderer::init() {
    Core::Logger::log(Core::LogLevel::Info, "OpenGL renderer initializing");
}

void OpenGLRenderer::beginScene() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::submitSprite() {

}

void OpenGLRenderer::endScene() {

}

} // RoseV2::OpenGL
