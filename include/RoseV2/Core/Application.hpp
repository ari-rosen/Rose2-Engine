#pragma once

#include "RoseV2/Core/Core.hpp"
#include "RoseV2/Core/Window.hpp"

#include "RoseV2/Graphics/Renderer.hpp"
#include "RoseV2/pch.hpp"

namespace RoseV2::Core {

class ROSE_V2_API Application {
public:
    Application();
    Application(const std::string& title, uint32_t width, uint32_t height);

    ~Application();

    void run();
private:
    void init(const std::string& title, uint32_t width, uint32_t height);
protected:
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<Graphics::Renderer> m_Renderer;
};

} // namespace RoseV2::Core


