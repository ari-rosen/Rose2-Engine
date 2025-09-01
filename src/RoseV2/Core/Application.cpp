#include "RoseV2/Core/Application.hpp"

namespace RoseV2::Core {

Application::Application() {
    init("RoseV2 Application", 1080, 680); 
}

Application::Application(const std::string& title, uint32_t width, uint32_t height) {
    init(title, width, height);
}


void Application::init(const std::string& title, uint32_t width, uint32_t height) {
    m_Window = Window::create(title, width, height);
    if (!m_Window) {
        throw std::runtime_error("Failed to create Window");
    }
}

} // namespace RoseV2::Core
