#include "RoseV2/Core/Application.hpp"
#include "RoseV2/Core/Logger.hpp"
#include "RoseV2/Graphics/GraphicsContext.hpp"
#include "RoseV2/Graphics/Renderer.hpp"

namespace RoseV2::Core {

Application::Application() {
    init("RoseV2 Application", 1080, 680); 
}

Application::Application(const std::string& title, uint32_t width, uint32_t height) {
    init(title, width, height);
}

Application::~Application() {
    Logger::log(LogLevel::Info, "Application shutting down");
    Graphics::Renderer::shutdown();
}


void Application::init(const std::string& title, uint32_t width, uint32_t height) {
#ifdef PROJECT_SOURCE_DIR
    std::filesystem::current_path(PROJECT_SOURCE_DIR);
#endif
    Logger::log(LogLevel::Info, "Application initalizing");
    
    Logger::log(LogLevel::Info, "Creating window");
    m_Window = Window::create(title, width, height);
    
    Graphics::Renderer::init();
}

void Application::run() {
    Logger::log(LogLevel::Info, "Application started");
    initImGui();

    while (!m_Window->shouldClose()) {
        m_Window->pollEvents();

        onRender();

        m_Window->swapBuffers();
    }
}

} // namespace RoseV2::Core
