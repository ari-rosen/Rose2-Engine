#pragma once

#include "RoseV2/Scene/Scene.hpp"

namespace RoseV2::Scene {

class SceneSerializer {
public:
    SceneSerializer(std::shared_ptr<Scene> scene);

    void serialize(const std::filesystem::path &filepath);
    bool deserialize(const std::filesystem::path &filepath);
private:
    std::shared_ptr<Scene> m_Scene;
};

}
