#pragma once

#include "RoseV2/pch.hpp"

namespace RoseV2::Core {

class Window {
public:
    static std::unique_ptr<Window> create(const std::string& title, uint32_t width, uint32_t height);
    
    virtual ~Window() = default;

    virtual void pollEvents() = 0;
    virtual void swapBuffers() = 0;
    virtual bool shouldClose() const = 0;

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;

    virtual void* getHandle() const = 0;
};

} // namespace RoseV2::Core
