#include "RoseV2/Graphics/UniformBuffer.hpp"
#include "RoseV2/Core/Logger.hpp"
#include "RoseV2/Graphics/RendererAPI.hpp"

#ifdef ROSEV2_USE_OPENGL
    #include "RoseV2/Platform/OpenGL/OpenGLUniformBuffer.hpp"
#endif

namespace RoseV2::Graphics {

std::shared_ptr<UniformBuffer> UniformBuffer::create(uint32_t size, uint32_t binding) {
    switch (RendererAPI::getAPI()) {
        case RendererAPI::API::OpenGL: return std::make_shared<Platform::OpenGL::OpenGLUniformBuffer>(size, binding);
        default: {
            Core::Logger::log(Core::LogLevel::Fatal, "Failed to create uniform buffer: No Graphics API definied");
            std::abort();
        }
    }
}

}
