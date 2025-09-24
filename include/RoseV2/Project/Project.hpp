#pragma once

#include "RoseV2/Asset/AssetManagerBase.hpp"
#include "RoseV2/Asset/EditorAssetManager.hpp"
#include <memory>

namespace RoseV2::Project {

struct ProjectData {
    std::string Name = "Untitled";
    std::filesystem::path StartScene;
    std::filesystem::path AssetDirectory;
};

class Project {
public:
    static const std::filesystem::path getProjectDirectory();
	static std::filesystem::path GetAssetDirectory();
    static std::filesystem::path GetAssetRegistryPath();

    static std::shared_ptr<Project> getActiveProject() { return s_ActiveProject; }

    static std::shared_ptr<Project> newProject();
    static std::shared_ptr<Project> loadProject(const std::filesystem::path &path);
    static bool saveActiveProject(const std::filesystem::path &path);

    ProjectData& getData() { return m_Data; };
    std::shared_ptr<Asset::AssetManagerBase> getAssetManager() { return m_AssetManager; }
    std::shared_ptr<Asset::EditorAssetManager> getEditorAssetManager() { return std::dynamic_pointer_cast<Asset::EditorAssetManager>(m_AssetManager); }
private:
	ProjectData m_Data;
    std::filesystem::path m_ProjectDirectory;
    std::shared_ptr<Asset::AssetManagerBase> m_AssetManager;

    inline static std::shared_ptr<Project> s_ActiveProject;
};

}
