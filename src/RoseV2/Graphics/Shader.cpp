#include "RoseV2/Graphics/Shader.hpp"

#include "RoseV2/Core/Logger.hpp"
#include "RoseV2/Graphics/RendererAPI.hpp"

#ifdef ROSEV2_USE_OPENGL
    #include "RoseV2/Platform/OpenGL/OpenGLShader.hpp"
#endif

namespace RoseV2::Graphics {

std::shared_ptr<Shader> Shader::create(std::filesystem::path vertPath, std::filesystem::path fragPath) {
    switch (RendererAPI::getAPI()) {
        case RendererAPI::API::OpenGL: return std::make_shared<Platform::OpenGL::OpenGLShader>(vertPath, fragPath);
        default: {
            Core::Logger::log(Core::LogLevel::Fatal, "Shader creation failed: No graphics api specified");
            std::abort();
        }
    }
}

}
