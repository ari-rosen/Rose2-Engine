#include "RoseV2/Project/Project.hpp"
#include "RoseV2/Asset/EditorAssetManager.hpp"
#include "RoseV2/Project/ProjectSerializer.hpp"

namespace RoseV2::Project {

const std::filesystem::path Project::getProjectDirectory() {
    return s_ActiveProject->m_ProjectDirectory;
}

std::filesystem::path Project::GetAssetDirectory() {
    return s_ActiveProject->m_Data.AssetDirectory;
}

std::filesystem::path Project::GetAssetRegistryPath() {
    return s_ActiveProject->m_ProjectDirectory / "Assets.roseres";
}

std::shared_ptr<Project> Project::newProject() {
    s_ActiveProject = std::make_shared<Project>();
    s_ActiveProject->m_AssetManager = std::make_shared<Asset::EditorAssetManager>();
	return s_ActiveProject;
}

std::shared_ptr<Project> Project::loadProject(const std::filesystem::path &path) {
    std::shared_ptr<Project> project = std::make_shared<Project>();
    
    ProjectSerializer serializer(project);
    if (serializer.deserialize(path)) {
        project->m_ProjectDirectory = path.parent_path();
        project->m_AssetManager = std::make_shared<Asset::EditorAssetManager>(); 
		s_ActiveProject = project;
		return s_ActiveProject;
    }

    return nullptr;
}

bool Project::saveActiveProject(const std::filesystem::path& path)
	{
		ProjectSerializer serializer(s_ActiveProject);
		if (serializer.serialize(path))
		{
			s_ActiveProject->m_ProjectDirectory = path.parent_path();
			return true;
		}

		return false;
	}

}
