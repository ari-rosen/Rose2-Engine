#include "RoseV2/Platform/GLFWWindow.hpp"
#include "RoseV2/Core/Assert.hpp"

#include "GLFW/glfw3.h"

namespace RoseV2::Platform {

GLFWWindow::GLFWWindow(const std::string& title, uint32_t width, uint32_t height) 
    : m_Width(width), m_Height(height)
{
    ROSEV2_ASSERT(glfwInit(), "Failed to Initialize GLFW");

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
   
    m_Handle = glfwCreateWindow(m_Width, m_Height, title.c_str(), nullptr, nullptr);
    ROSEV2_ASSERT(m_Handle, "Failed to Create GLFW Window"); 

    glfwMakeContextCurrent(m_Handle);
}

GLFWWindow::~GLFWWindow() {
    if (m_Handle) {
        glfwDestroyWindow(m_Handle);
    }
    glfwTerminate();
}

void GLFWWindow::pollEvents() {
    glfwPollEvents();
}

void GLFWWindow::swapBuffers() {
    glfwSwapBuffers(m_Handle);
}

bool GLFWWindow::shouldClose() const {
    return glfwWindowShouldClose(m_Handle);
}

uint32_t GLFWWindow::getWidth() const {
    return m_Width;
}

uint32_t GLFWWindow::getHeight() const {
    return m_Height;
}

} // namespace RoseV2::Platform
