#pragma once

#include "RoseV2/Core/AssetManager.hpp"
#include "RoseV2/Core/Core.hpp"
#include "RoseV2/Core/Window.hpp"

namespace RoseV2::Core {

class ROSE_V2_API Application {
public:
    Application();
    Application(const std::string& title, uint32_t width, uint32_t height);

    ~Application();

    void run();
    void stop();

    virtual void initImGui() {}; // for editor
    virtual void onRender() {};
private:
    void init(const std::string& title, uint32_t width, uint32_t height);
protected:
    std::unique_ptr<Window> m_Window;
    bool m_Running;
};

} // namespace RoseV2::Core


