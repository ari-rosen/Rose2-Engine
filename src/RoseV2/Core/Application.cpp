#include "RoseV2/Core/Application.hpp"
#include "RoseV2/Core/Logger.hpp"
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
}


void Application::init(const std::string& title, uint32_t width, uint32_t height) {
    Logger::log(LogLevel::Info, "Application initalizing");
    
    Logger::log(LogLevel::Info, "Creating window");
    m_Window = Window::create(title, width, height);
    if (!m_Window) {
        throw std::runtime_error("Failed to create Window");
    }
    
    Logger::log(LogLevel::Info, "Creating Renderer");
    m_Renderer = Graphics::Renderer::create();
}

void Application::run() {
    Logger::log(LogLevel::Info, "Application started");
    while (!m_Window->shouldClose()) {
        m_Window->pollEvents();
    }
}

} // namespace RoseV2::Core
