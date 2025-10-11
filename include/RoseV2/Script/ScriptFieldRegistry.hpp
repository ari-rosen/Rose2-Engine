#pragma once

#include "RoseV2/Core/UUID.hpp"
#include "RoseV2/ECS/Entity.hpp"
#include "RoseV2/Scene/Scene.hpp"
namespace RoseV2::Script {

enum class FieldType { Number, Bool, String };

struct SerializableField {
    std::string Name;
    FieldType Type;
    std::variant<double, bool, std::string> DefaultValue;
};

class ScriptFieldRegistry {
public:
    // Maps an UUID to a Map which maps a (script) name to a list of fields
    using UUIDFieldMap = std::unordered_map<
        uint64_t, 
        std::unordered_map<std::string, std::vector<SerializableField>>>;
    
    // Maps a script's name to its default fields
    using DefaultFieldMap = std::unordered_map<std::string, std::vector<SerializableField>>;
    
    void registerDefaultSerializableFields(const std::string &scriptName);
    void syncEntityFields(const std::string &scriptName, Scene::Scene &scene);
    std::vector<SerializableField>& getEntitySerializeableFields(Core::UUID uuid, const std::string &scriptName);

    void serializeEntityFields(Scene::Scene &scene);
    bool deserializeEntityFields(Scene::Scene &scene);
private:
    UUIDFieldMap m_EntityFields{}; // Per entity overloads
    DefaultFieldMap m_DefaultFields{}; // default script fields
};

}
