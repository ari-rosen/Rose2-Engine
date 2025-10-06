#pragma once

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
    // Maps an Entity to a Map which maps a (script) name to a list of fields
    using EntityFieldMap = std::unordered_map<
        ECS::Entity, 
        std::unordered_map<std::string, std::vector<SerializableField>>, 
        ECS::EntityHash>;
    
    // Maps a script's name to its default fields
    using DefaultFieldMap = std::unordered_map<std::string, std::vector<SerializableField>>;
    
    static void registerDefaultSerializableFields(const std::string &scriptName);
    static void syncEntityFields(const std::string &scriptName, Scene::Scene &scene);
    static std::vector<SerializableField>& getEntitySerializeableFields(ECS::Entity entity, const std::string &scriptName);

    static void serializeEntityFields(Scene::Scene &scene);
    static bool deserializeEntityFields(Scene::Scene &scene);
private:
    static EntityFieldMap s_EntityFields; // Per entity overloads
    static DefaultFieldMap s_DefaultFields; // default script fields
};

}
