#pragma once

#include "RoseV2/Core/Application.hpp"
#include "RoseV2/Core/Core.hpp"

namespace RoseV2::Core {

class ROSE_V2_API Application2D : public Application {
public:
    Application2D();
    Application2D(const std::string& title, uint32_t width, uint32_t height);
    
    void run() override;
};

} // namespace RoseV2::Core
