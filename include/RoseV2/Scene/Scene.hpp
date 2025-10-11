#pragma once

#include "RoseV2/Core/Timestep.hpp"
#include "RoseV2/Core/UUID.hpp"
#include "RoseV2/ECS/Registry.hpp"
#include "RoseV2/Core/Core.hpp"

namespace RoseV2::Scene {

class ROSE_V2_API Scene {
public:
    Scene();

    void onRuntimeStart();
    void onRuntimeUpdate(Core::Timestep ts);
    void onRuntimeStop();

    ECS::Entity getEntityFromUUID(const Core::UUID &uuid);
   
    inline ECS::Registry& getRegistry() { return m_Registry; }
    
    const std::string &getName() const { return m_Name; }
    void setName(const std::string &name) { m_Name = name; }
private:
    bool m_IsRunning;
    std::string m_Name;

    ECS::Registry m_Registry;
};

}
