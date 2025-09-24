#include "RoseV2/Project/ProjectSerializer.hpp"
#include "RoseV2/Project/Project.hpp"

#include <json.hpp>
#include <fstream>

namespace RoseV2::Project {

using json = nlohmann::json;

ProjectSerializer::ProjectSerializer(std::shared_ptr<Project> project)
    : m_Project(project)
{}

bool ProjectSerializer::serialize(const std::filesystem::path &filepath) {
    if (!m_Project) return false;

    json j;
    const ProjectData& data = m_Project->getData();

    j["Name"] = data.Name;
    j["StartScene"] = data.StartScene.string();
    j["AssetDirectory"] = data.AssetDirectory.string();

    std::ofstream file(filepath);
    if (!file.is_open()) return false;

    file << j.dump(4);
    return true;
}

bool ProjectSerializer::deserialize(const std::filesystem::path &filepath) {
    if (!m_Project) return false;

    std::ifstream file(filepath);
    if (!file.is_open()) return false;
    
    json j;
    file >> j;

    ProjectData& data = m_Project->getData();
    data.Name = j.value("Name", "Untitled");
    data.StartScene = j.value("StartScene", "");
    data.AssetDirectory = j.value("AssetDirectory", "");

    return true;
}

} // namespace RoseV2::Project
