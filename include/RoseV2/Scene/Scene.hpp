#pragma once

#include "RoseV2/ECS/Registry.hpp"
#include "RoseV2/Core/Core.hpp"

namespace RoseV2::Scene {

class ROSE_V2_API Scene {
public:
    Scene();

    void onStart();
    void onStop();
    
    void onRuntimeUpdate();
   
    inline ECS::Registry& getRegistry() { return m_Registry; }
private:
    bool m_IsRunning;
    ECS::Registry m_Registry;
};

}
