#include "RoseV2/Script/ScriptFieldRegistry.hpp"
#include "RoseV2/Core/UUID.hpp"
#include "RoseV2/ECS/Component.hpp"
#include "RoseV2/ECS/Entity.hpp"
#include "RoseV2/Project/Project.hpp"
#include "RoseV2/Scene/Scene.hpp"
#include "RoseV2/Script/LuaEngine.hpp"
#include "RoseV2/Script/LuaHeaders.hpp"

#include <json.hpp>

namespace RoseV2::Script {

using json = nlohmann::json;

static std::string fieldTypeToString(FieldType type) {
    switch (type) {
        case FieldType::Number: return "Number";
        case FieldType::Bool: return "Bool";
        case FieldType::String: return "String";
    }
}

void ScriptFieldRegistry::registerDefaultSerializableFields(const std::string& scriptName) {
    lua_State* L = LuaEngine::getLuaState();

    lua_getglobal(L, scriptName.c_str());
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        return;
    }

    std::vector<SerializableField> defaults;

    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {
        const char* fieldName = lua_tostring(L, -2);
        if (!fieldName) { lua_pop(L, 1); continue; }

        if (lua_isnumber(L, -1))
            defaults.push_back({fieldName, FieldType::Number, lua_tonumber(L, -1)});
        else if (lua_isboolean(L, -1))
            defaults.push_back({fieldName, FieldType::Bool, (bool)lua_toboolean(L, -1)});
        else if (lua_isstring(L, -1))
            defaults.push_back({fieldName, FieldType::String, lua_tostring(L, -1)});

        lua_pop(L, 1);
    }

    m_DefaultFields[scriptName] = defaults;

    lua_pop(L, 1);
}

void ScriptFieldRegistry::syncEntityFields(const std::string& scriptName, Scene::Scene &scene) {
    auto defIt = m_DefaultFields.find(scriptName);
    if (defIt == m_DefaultFields.end()) return;

    const auto& newDefaults = defIt->second;
    auto& registry = scene.getRegistry();

    for (auto it = m_EntityFields.begin(); it != m_EntityFields.end(); ++it) {
        uint64_t uuid = it->first;
        ECS::Entity entity = scene.getEntityFromUUID(uuid);

        if (entity == ECS::Entity::Null() || !registry.isEntityAlive(entity)) {
            it = m_EntityFields.erase(it);
            continue;
        }

        if (!registry.hasComponent<ECS::UUIDComponent>(entity)) {
            registry.addComponent(entity, ECS::UUIDComponent{Core::UUID{}});
        }

        auto& scriptMap = it->second;
        auto scriptIt = scriptMap.find(scriptName);
        if (scriptIt == scriptMap.end()) continue;

        auto& currentFields = scriptIt->second;

        currentFields.erase(
            std::remove_if(currentFields.begin(), currentFields.end(),
                [&](const SerializableField& f){
                    return std::none_of(newDefaults.begin(), newDefaults.end(),
                        [&](const SerializableField& def){ return def.Name == f.Name; });
                }),
            currentFields.end()
        );

        for (const auto& defField : newDefaults) {
            auto found = std::find_if(currentFields.begin(), currentFields.end(),
                [&](const SerializableField& f){ return f.Name == defField.Name; });

            if (found == currentFields.end()) {
                currentFields.push_back(defField);
            }
        }
    }
}

std::vector<SerializableField>& ScriptFieldRegistry::getEntitySerializeableFields(Core::UUID uuid, const std::string &scriptName) {
    auto& entityScriptMap = m_EntityFields[(uint64_t)uuid];

    auto scriptIt = entityScriptMap.find(scriptName);

    if (scriptIt != entityScriptMap.end()) {
        return scriptIt->second;
    }

    auto defIt = m_DefaultFields.find(scriptName);

    if (defIt != m_DefaultFields.end()) {
        entityScriptMap[scriptName] = defIt->second;
        return entityScriptMap[scriptName];
    }

    Core::Logger::log(Core::LogLevel::Warning, "No default fields for script: " + scriptName);
    
    return entityScriptMap[scriptName]; 
}

void ScriptFieldRegistry::serializeEntityFields(Scene::Scene &scene) {
    auto path = Project::Project::getProjectDirectory() / "Fields.rosefields";
    json j;

    for (const auto&[uuid, fieldmap] : m_EntityFields) {
        ECS::Entity entity = scene.getEntityFromUUID(uuid);
        if (entity == ECS::Entity::Null()) {
            continue;
        }
        if (!scene.getRegistry().isEntityAlive(entity)) continue;

        json entityJson;
        
        // UUID
        if (!scene.getRegistry().hasComponent<ECS::UUIDComponent>(entity)) {
            scene.getRegistry().addComponent(entity, ECS::UUIDComponent{Core::UUID{}});    
        }

        entityJson["UUID"] = uuid;

        for (const auto&[scriptName, fields] : fieldmap) {
            // Script Name
            json scriptJson;
            scriptJson["ScriptName"] = scriptName;

            for (const auto& field : fields) {
                // Fields
                json fieldJson;
                fieldJson["Name"] = field.Name;
                fieldJson["Type"] = fieldTypeToString(field.Type);
                
                switch (field.Type) {
                    case FieldType::Number: {
                        fieldJson["Value"] = std::get<double>(field.DefaultValue);
                        break;
                    }
                    case FieldType::Bool: {
                        fieldJson["Value"] = std::get<bool>(field.DefaultValue);
                        break;
                    }
                    case FieldType::String: {
                        fieldJson["Value"] = std::get<std::string>(field.DefaultValue);
                        break;
                    }
                }

                scriptJson["Fields"].push_back(fieldJson);
            }
            
            entityJson["Scripts"].push_back(scriptJson);
        }

        j["Entities"].push_back(entityJson);
    }

    std::ofstream file(path);
    if (file.is_open()) {
        file << j.dump(4);
    }
}


bool ScriptFieldRegistry::deserializeEntityFields(Scene::Scene &scene) {
    auto path = Project::Project::getProjectDirectory() / "Fields.rosefields";
    std::ifstream file(path);

    if (!file.is_open()) {
        return false;
    }

    json j;
    try {
        file >> j;
    } catch (const std::exception& e) {
        Core::Logger::log(Core::LogLevel::Error, "Failed to parse Fields.rosefields: " + std::string(e.what()));
        return false;
    }

    auto& registry = scene.getRegistry();

    if (!j.contains("Entities") || !j["Entities"].is_array()) {
        return false;
    }

    for (const auto& entityJson : j["Entities"]) {
        if (!entityJson.contains("UUID")) continue;

        uint64_t uuid = entityJson["UUID"].get<uint64_t>();
        ECS::Entity entity{};
        bool found = false;

        // Find the entity in the scene by UUID
        auto view = registry.view<ECS::UUIDComponent>();
        for (auto e : view) {
            auto& comp = registry.getComponent<ECS::UUIDComponent>(e);
            if (comp.ID == uuid) {
                entity = e;
                found = true;
                break;
            }
        }

        if (!found) continue;

        if (!entityJson.contains("Scripts") || !entityJson["Scripts"].is_array()) continue;

        for (const auto& scriptJson : entityJson["Scripts"]) {
            if (!scriptJson.contains("ScriptName")) continue;
            std::string scriptName = scriptJson["ScriptName"].get<std::string>();

            std::vector<SerializableField> fields;
            if (scriptJson.contains("Fields") && scriptJson["Fields"].is_array()) {
                for (const auto& fieldJson : scriptJson["Fields"]) {
                    SerializableField field;
                    field.Name = fieldJson.value("Name", "");
                    std::string typeStr = fieldJson.value("Type", "Number");

                    if (typeStr == "Number") {
                        field.Type = FieldType::Number;
                        field.DefaultValue = fieldJson.value("Value", 0.0);
                    } else if (typeStr == "Bool") {
                        field.Type = FieldType::Bool;
                        field.DefaultValue = fieldJson.value("Value", false);
                    } else if (typeStr == "String") {
                        field.Type = FieldType::String;
                        field.DefaultValue = fieldJson.value("Value", "");
                    }

                    fields.push_back(field);
                }
            }

            m_EntityFields[uuid][scriptName] = fields;
        }
    }

    return true;
}




}
