#include "RoseV2/Graphics/Renderer.hpp"
#include "RoseV2/Core/Logger.hpp"

#ifdef ROSEV2_USE_OPENGL
    #include "RoseV2/Graphics/OpenGLRenderer.hpp"
#endif

namespace RoseV2::Graphics {

std::unique_ptr<Renderer> Renderer::create() {
#ifdef ROSEV2_USE_OPENGL
    return std::make_unique<OpenGLRenderer>(); 
#else
    Core::Logger::log(Core::LogLevel::Fatal, "Renderer initalizing failed: No Graphics Api Defined");
    std::abort();
#endif

}

}
