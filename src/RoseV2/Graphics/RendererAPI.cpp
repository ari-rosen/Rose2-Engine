#include "RoseV2/Graphics/RendererAPI.hpp"
#include "RoseV2/Core/Logger.hpp"

#ifdef ROSEV2_USE_OPENGL
    #include "RoseV2/Platform/OpenGL/OpenGLRendererAPI.hpp"
#endif

namespace RoseV2::Graphics {

RendererAPI::API RendererAPI::s_API = RendererAPI::API::None;

std::unique_ptr<RendererAPI> RendererAPI::create() {

#ifdef ROSEV2_USE_OPENGL
    s_API = API::OpenGL;
    return std::make_unique<Platform::OpenGL::OpenGLRendererAPI>();
#else
    Core::Logger::log(Core::LogLevel::Fatal, "Renderer API initialization falied, no graphics api defined");
    std::abort();
#endif

}

}
