#include "RoseV2/Platform/OpenGL/OpenGLGraphicsContext.hpp"

#include "RoseV2/Core/Assert.hpp"
#include "RoseV2/Core/Logger.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace RoseV2::Platform::OpenGL {
    
void OpenGLGraphicsContext::init() { 
    Core::Logger::log(Core::LogLevel::Info, "OpenGL context initializing");

    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    ROSEV2_ASSERT(status, "OpenGL Context Initialization failed: Failed to load GLAD");
}

}
