#include "RoseV2/Graphics/Texture.hpp"

#include "RoseV2/Core/Logger.hpp"
#include "RoseV2/Graphics/RendererAPI.hpp"

#ifdef ROSEV2_USE_OPENGL
    #include "RoseV2/Platform/OpenGL/OpenGLTexture.hpp"
#endif

namespace RoseV2::Graphics {

std::shared_ptr<Texture> Texture::create(const std::filesystem::path &path) {
    switch (RendererAPI::getAPI()) {
        case RendererAPI::API::OpenGL: return std::make_shared<Platform::OpenGL::OpenGLTexture>(path);
        default: {
            Core::Logger::log(Core::LogLevel::Fatal, "Failed to create texture: No Graphics API Defined");
            std::abort();
        }
    }
}

std::shared_ptr<Texture> Texture::create(const TextureSpecification &spec) {
    switch (RendererAPI::getAPI()) {
        case RendererAPI::API::OpenGL: return std::make_shared<Platform::OpenGL::OpenGLTexture>(spec);
        default: {
            Core::Logger::log(Core::LogLevel::Fatal, "Failed to create texture: No Graphics API Defined");
            std::abort();
        }
    }
}

}
