#pragma once

#include "RoseV2/Asset/Asset.hpp"
#include "RoseV2/Core/Timestep.hpp"
#include "RoseV2/ECS/Entity.hpp"
#include "RoseV2/Script/LuaRef.hpp"

namespace RoseV2::Script {

class ScriptInstance : public Asset::AssetBase {
public:
    ScriptInstance(const std::string &name);

    bool loadSource(const std::vector<std::byte> &bytecode);

    void invokeOnStart(ECS::Entity e);
    void invokeOnUpdate(ECS::Entity e, Core::Timestep ts);

    Asset::AssetType getAssetType() const override { return Asset::AssetType::Script; }
    inline const std::string& getName() const { return m_Name; }
private:
    bool pushFunction(const char *functionName);
    bool pushEntity(ECS::Entity e);
private:
    std::string m_Name;

    LuaRef m_TableRef{};
};

};
