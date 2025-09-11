#include "RoseV2/Graphics/VertexArray.hpp"
#include "RoseV2/Core/Logger.hpp"
#include "RoseV2/Graphics/RendererAPI.hpp"

#ifdef ROSEV2_USE_OPENGL
    #include "RoseV2/Platform/OpenGL/OpenGLVertexArray.hpp"
#endif

namespace RoseV2::Graphics {

std::shared_ptr<VertexArray> VertexArray::create() {
    switch (RendererAPI::getAPI()) {
        case RendererAPI::API::OpenGL: return std::make_shared<Platform::OpenGL::OpenGLVertexArray>();
        default: {
            Core::Logger::log(Core::LogLevel::Fatal, "Failed to create Vertex Array: No graphics API defined");
            std::abort();
        }
    }
}

}
