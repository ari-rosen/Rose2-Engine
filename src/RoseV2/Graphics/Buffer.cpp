#include "RoseV2/Graphics/Buffer.hpp"
#include "RoseV2/Core/Logger.hpp"
#include "RoseV2/Graphics/RendererAPI.hpp"

#ifdef ROSEV2_USE_OPENGL
    #include "RoseV2/Platform/OpenGL/OpenGLBuffer.hpp"
#endif

namespace RoseV2::Graphics {

std::shared_ptr<VertexBuffer> VertexBuffer::create(uint32_t size) {
    switch (RendererAPI::getAPI()) {
        case RendererAPI::API::OpenGL: return std::make_shared<Platform::OpenGL::OpenGLVertexBuffer>(size);
        default: {
            Core::Logger::log(Core::LogLevel::Fatal, "Failed to create Vertex Buffer: No graphics API defined");
            std::abort();
        }
    }
}

std::shared_ptr<VertexBuffer> VertexBuffer::create(float *vertices, uint32_t size) {
    switch (RendererAPI::getAPI()) {
        case RendererAPI::API::OpenGL: return std::make_shared<Platform::OpenGL::OpenGLVertexBuffer>(vertices, size);
        default: {
            Core::Logger::log(Core::LogLevel::Fatal, "Failed to create Vertex Buffer: No graphics API defined");
            std::abort();
        }
    }
}

std::shared_ptr<IndexBuffer> IndexBuffer::create(uint32_t *indices, uint32_t size) {
    switch (RendererAPI::getAPI()) {
        case RendererAPI::API::OpenGL: return std::make_shared<Platform::OpenGL::OpenGLIndexBuffer>(indices, size);
        default: {
            Core::Logger::log(Core::LogLevel::Fatal, "Failed to create Index Buffer: No graphics API defined");
            std::abort();
        }
    }
}

}
