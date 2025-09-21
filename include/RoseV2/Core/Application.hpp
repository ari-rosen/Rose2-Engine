#pragma once

#include "RoseV2/Core/Core.hpp"
#include "RoseV2/Core/Layer.hpp"
#include "RoseV2/Core/Timestep.hpp"
#include "RoseV2/Core/Window.hpp"

namespace RoseV2::Core {

class ROSE_V2_API Application {
public:
    Application();
    Application(const std::string& title, uint32_t width, uint32_t height);

    ~Application();

    void pushLayer(std::unique_ptr<Layer> layer);

    void run();
    void stop();

    const std::unique_ptr<Window>& getWindow() const { return m_Window; }
private:
    void init(const std::string& title, uint32_t width, uint32_t height);
    Timestep getDeltaTime();
private:
    std::vector<std::unique_ptr<Layer>> m_LayerStack;
    std::unique_ptr<Window> m_Window;
    bool m_Running;
};

} // namespace RoseV2::Core


