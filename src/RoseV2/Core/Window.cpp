#include "RoseV2/Core/Window.hpp"

#ifdef ROSEV2_USE_GLFW
    #include "RoseV2/Platform/GLFWWindow.hpp"
#endif

namespace RoseV2::Core {

std::unique_ptr<Window> Window::create(const std::string& title, uint32_t width, uint32_t height) {
#ifdef ROSEV2_USE_GLFW
    return std::make_unique<RoseV2::Platform::GLFWWindow>(title, width, height);
#else
    throw std::runtime_error("No window backend defined");
#endif
}

} // namespace RoseV2::Core
