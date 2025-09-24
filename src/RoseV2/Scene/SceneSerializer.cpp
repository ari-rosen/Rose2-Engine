#include "RoseV2/Scene/SceneSerializer.hpp"

#include "RoseV2/ECS/Component.hpp"

#include <json.hpp>

namespace RoseV2::Scene {

using json = nlohmann::json;

SceneSerializer::SceneSerializer(std::shared_ptr<Scene> scene)
    :m_Scene(scene)
{}

void SceneSerializer::serialize(const std::filesystem::path &filepath) {
    json j;

    j["Name"] = m_Scene->getName();
    
    for (auto e : m_Scene->getRegistry().view<ECS::TagComponent>()) {
        if (!m_Scene->getRegistry().isEntityAlive(e))
            continue;

        json entityJson;

        if (m_Scene->getRegistry().hasComponent<ECS::TagComponent>(e)) {
            const auto &tag = m_Scene->getRegistry().getComponent<ECS::TagComponent>(e);
            entityJson["TagComponent"]["Tag"] = tag.Tag;
        }
        if (m_Scene->getRegistry().hasComponent<ECS::TransformComponent>(e)) {
            const auto &t = m_Scene->getRegistry().getComponent<ECS::TransformComponent>(e);
            entityJson["TransformComponent"]["Translation"] = {t.Translation.x, t.Translation.y, t.Translation.z};
            entityJson["TransformComponent"]["Rotation"] = {t.Rotation.x, t.Rotation.y, t.Rotation.z};
            entityJson["TransformComponent"]["Scale"] = {t.Scale.x, t.Scale.y, t.Scale.z};
        }
        if (m_Scene->getRegistry().hasComponent<ECS::SpriteComponent>(e)) {
            const auto& s = m_Scene->getRegistry().getComponent<ECS::SpriteComponent>(e);
            entityJson["SpriteComponent"]["Color"] = { s.Color.r, s.Color.g, s.Color.b, s.Color.a };
            entityJson["SpriteComponent"]["TextureHandle"] = (uint64_t)s.TextureHandle; 
        }

        if (m_Scene->getRegistry().hasComponent<ECS::CameraComponent>(e)) {
            const auto& c = m_Scene->getRegistry().getComponent<ECS::CameraComponent>(e);
            entityJson["CameraComponent"]["Zoom"] = c.Zoom;
            entityJson["CameraComponent"]["Left"] = c.Left;
            entityJson["CameraComponent"]["Right"] = c.Right;
            entityJson["CameraComponent"]["Bottom"] = c.Bottom;
            entityJson["CameraComponent"]["Top"] = c.Top;
            entityJson["CameraComponent"]["Primary"] = c.Primary;
        }
        
        j["Entities"].push_back(entityJson);
    }

    std::ofstream file(filepath);
    if (file.is_open()) {
        file << j.dump(4);
    }
}

bool SceneSerializer::deserialize(const std::filesystem::path &filepath) {
    std::ifstream file(filepath);
    if (!file.is_open())
        return false;

    json j;
    file >> j;

    m_Scene->setName(j["Name"]);
    
    for (const auto& entityJson : j["Entities"]) {
        ECS::Entity e = m_Scene->getRegistry().createEntity();

        if (entityJson.contains("TagComponent")) {
            m_Scene->getRegistry().addComponent(e, ECS::TagComponent{
                entityJson["TagComponent"]["Tag"].get<std::string>()
            });
        }

        if (entityJson.contains("TransformComponent")) {
            const auto& t = entityJson["TransformComponent"];
            m_Scene->getRegistry().addComponent(e, ECS::TransformComponent{
                { t["Translation"][0], t["Translation"][1], t["Translation"][2] },
                { t["Rotation"][0], t["Rotation"][1], t["Rotation"][2] },
                { t["Scale"][0], t["Scale"][1], t["Scale"][2] }
            });
        }

        if (entityJson.contains("SpriteComponent")) {
            const auto& s = entityJson["SpriteComponent"];

            m_Scene->getRegistry().addComponent(e, ECS::SpriteComponent{
                { s["Color"][0], s["Color"][1], s["Color"][2], s["Color"][3] },
                s["TextureHandle"].get<uint64_t>()
            });
        }

        if (entityJson.contains("CameraComponent")) {
            const auto& c = entityJson["CameraComponent"];
            m_Scene->getRegistry().addComponent(e, ECS::CameraComponent{
                c["Zoom"], c["Left"], c["Right"], c["Bottom"], c["Top"], c["Primary"]
            });
        }
    }

    return true;
}

}
