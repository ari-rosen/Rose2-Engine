#pragma once

#include "RoseV2/ECS/Registry.hpp"

namespace RoseV2::Scene {

class Scene {
public:
    Scene();

    void onStart();
    void onStop();
    
    void onRuntimeUpdate();
   
    inline const ECS::Registry& getRegistry() { return m_Registry; }
private:
    bool m_IsRunning;
    ECS::Registry m_Registry;
};

}
