#include "RoseV2/Graphics/Framebuffer.hpp"
#include "RoseV2/Core/Logger.hpp"
#include "RoseV2/Graphics/RendererAPI.hpp"

#ifdef ROSEV2_USE_OPENGL
    #include "RoseV2/Platform/OpenGL/OpenGLFramebuffer.hpp"
#endif

namespace RoseV2::Graphics {

std::shared_ptr<Framebuffer> Framebuffer::create(const FramebufferSpecification &spec) {

    switch (RendererAPI::getAPI()) {
        case RendererAPI::API::OpenGL: return std::make_shared<Platform::OpenGL::OpenGLFramebuffer>(spec);
        default: {
            Core::Logger::log(Core::LogLevel::Fatal, "Failed to create framebuffer: No Graphics API Defined");
            std::abort();
        }
    }

}

}
