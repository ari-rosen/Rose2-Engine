#include "RoseV2/Graphics/OpenGLRenderer.hpp"

#include "RoseV2/Core/Assert.hpp"
#include "RoseV2/Core/Logger.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace RoseV2::Graphics {

void OpenGLRenderer::init() {
    Core::Logger::log(Core::LogLevel::Info, "OpenGL renderer initializing");
    ROSEV2_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "OpenGL Renderer Initialization failed: Failed to load GLAD");
}

void OpenGLRenderer::beginScene() {

}

void OpenGLRenderer::submitSprite() {

}

void OpenGLRenderer::renderScene() {

}

void OpenGLRenderer::endScene() {

}

} // RoseV2::Graphics
