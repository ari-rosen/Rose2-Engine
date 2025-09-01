#include "RoseV2/Core/Application.hpp"
#include "RoseV2/Core/Application2D.hpp"

namespace RoseV2::Core {

Application2D::Application2D() 
    : Application()
{}

Application2D::Application2D(const std::string& title, uint32_t width, uint32_t height)
    : Application(title, width, height)
{}

void Application2D::run() {
    while (!m_Window->shouldClose()) {
        m_Window->pollEvents();
    }
}

}
