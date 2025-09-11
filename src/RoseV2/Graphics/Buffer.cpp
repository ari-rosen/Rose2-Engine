#include "RoseV2/Graphics/Buffer.hpp"
#include "RoseV2/Core/Logger.hpp"
#include "RoseV2/Graphics/RendererAPI.hpp"

#ifdef ROSEV2_USE_OPENGL
    #include "RoseV2/Platform/OpenGL/OpenGLBuffer.hpp"
#endif

namespace RoseV2::Graphics {

inline uint32_t ShaderDataTypeSize(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Float:     return 4;
        case ShaderDataType::Float2:    return 4 * 2;
        case ShaderDataType::Float3:    return 4 * 3;
        case ShaderDataType::Float4:    return 4 * 4;
        case ShaderDataType::Int:       return 4;
        case ShaderDataType::Int2:      return 4 * 2;
        case ShaderDataType::Int3:      return 4 * 3;
        case ShaderDataType::Int4:      return 4 * 4;
        case ShaderDataType::Bool:      return 1;
        case ShaderDataType::Mat3:      return 4 * 3 * 3;
        case ShaderDataType::Mat4:      return 4 * 4 * 4;
    }
    return 0;
}

BufferElement::BufferElement(ShaderDataType type, const std::string &name, bool normalized)
    : Name(name), Type(type), Normalized(normalized), Size(ShaderDataTypeSize(type))
{}

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
    : Elements(elements)
{
    uint32_t offset = 0;
    Stride = 0;

    for (auto& element : Elements) {
        element.Offset = offset;
        offset += element.Size;
        Stride += element.Size;
    }

    uint32_t currentLocation = 0;
    for (auto& element : Elements) {
        element.Location = currentLocation;

        switch (element.Type) {
            case ShaderDataType::Mat3: currentLocation += 3; break;
            case ShaderDataType::Mat4: currentLocation += 4; break;
            default: currentLocation += 1; break;
        }
}
}


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
