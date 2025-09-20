#include "RoseV2/Project/ProjectSerializer.hpp"

#include <json.hpp>

namespace RoseV2::Project {

using json = nlohmann::json;

ProjectSerializer::ProjectSerializer(std::shared_ptr<Project> project)
    :m_Project(project)
{}

bool ProjectSerializer::serialize(const std::filesystem::path &filepath) {
    json j;

    j["Name"] = m_Project->Name;
    j["StartScene"] = m_Project->StartScene;
    j["AssetDirectory"] = m_Project->AssetDirectory;

    std::ofstream file(filepath);
    if (file.is_open()) {
        file << j.dump(4);
    }
    else return false;

    return true;
}

bool ProjectSerializer::deserialize(const std::filesystem::path &filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;
    
    json j;
    file >> j;

    m_Project->Name = j.value("Name", "");
    m_Project->StartScene = j.value("StartScene", "");
    m_Project->AssetDirectory = j.value("AssetDirectory", "");

    return true;
}

}
