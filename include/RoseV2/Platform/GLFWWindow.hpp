#pragma once

#include "RoseV2/Core/Window.hpp"
#include "RoseV2/Graphics/GraphicsContext.hpp"

class GLFWwindow;

namespace RoseV2::Platform {

class GLFWWindow : public RoseV2::Core::Window {
public:
    GLFWWindow(const std::string& title, uint32_t width, uint32_t height);
    ~GLFWWindow() override;

    void pollEvents() override;
    void swapBuffers() override;
    bool shouldClose() const override;

    uint32_t getWidth() const override;
    uint32_t getHeight() const override;
private:
    GLFWwindow *m_Handle;
    uint32_t m_Width, m_Height;
    
    std::unique_ptr<Graphics::GraphicsContext> m_Context;
};

}
