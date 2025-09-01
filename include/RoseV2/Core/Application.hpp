#pragma once

#include "RoseV2/Core/Core.hpp"
#include "RoseV2/Core/Window.hpp"

#include "RoseV2/pch.hpp"

namespace RoseV2::Core {

class Application {
public:
    Application();
    Application(const std::string& title, uint32_t width, uint32_t height);

    virtual void run() = 0;
private:
    void init(const std::string& title, uint32_t width, uint32_t height);
protected:
    std::unique_ptr<Window> m_Window;
};

} // namespace RoseV2::Core


