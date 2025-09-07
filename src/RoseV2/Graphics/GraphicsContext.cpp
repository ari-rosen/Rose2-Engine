#include "RoseV2/Graphics/GraphicsContext.hpp"

#ifdef ROSEV2_USE_OPENGL
    #include "RoseV2/Platform/OpenGL/OpenGLGraphicsContext.hpp"
#endif

namespace RoseV2::Graphics {

std::unique_ptr<GraphicsContext> GraphicsContext::create() {

#ifdef ROSEV2_USE_OPENGL
    return std::make_unique<Platform::OpenGL::OpenGLGraphicsContext>(); 
#else
    Core::Logger::log(Core::LogLevel::Fatal, "Graphics Contexts initalizing failed: No Graphics Api Defined");
    std::abort();
#endif

}

} // namespace RoseV2::Graphics
