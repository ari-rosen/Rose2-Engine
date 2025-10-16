#pragma once

#include "RoseV2/Core/UUID.hpp"
#include "RoseV2/Script/LuaRef.hpp"
#include "RoseV2/Scene/Scene.hpp"

namespace RoseV2::Script {

class RuntimeEntityTables {
public:
    using UUIDTableMap = std::unordered_map<uint64_t, std::unordered_map<std::string, LuaRef>>;

    void ensureEntityTable(Core::UUID uuid, const std::string& scriptName);
    LuaRef& getEntityTable(Core::UUID uuid, const std::string& scriptName);
    
    void cleanup(Scene::Scene& scene);
    void removeEntity(Core::UUID uuid);
private:
    UUIDTableMap m_EntityTables;
};

}


