#include "RoseV2/Asset/EditorAssetManager.hpp"
#include "RoseV2/Asset/Asset.hpp"
#include "RoseV2/Asset/AssetImporter.hpp"
#include "RoseV2/Core/Logger.hpp"
#include "RoseV2/Project/Project.hpp"

#include <json.hpp>

namespace RoseV2::Asset {

using json = nlohmann::json;

static std::map<std::filesystem::path, AssetType> s_AssetExtensionMap = {
	{ ".png", AssetType::Texture },
	{ ".jpg", AssetType::Texture },
	{ ".jpeg",AssetType::Texture }
};

static AssetType getAssetTypeFromFileExtension(const std::filesystem::path& extension) {
	if (s_AssetExtensionMap.find(extension) == s_AssetExtensionMap.end()) {
        Core::Logger::log(Core::LogLevel::Error, "Couldn't find asset type for loaded asset");
		return AssetType::None;
	}

	return s_AssetExtensionMap.at(extension);
}

static std::string assetTypeToString(AssetType type) {
    switch (type) {
        case AssetType::None: return "None";
        case AssetType::Texture: return "Texture";
        default: return "None";
    }
}

bool EditorAssetManager::isAssetHandleValid(AssetHandle handle) const {
    return m_AssetRegistry.contains(handle);
}

bool EditorAssetManager::isAssetHandleLoaded(AssetHandle handle) const {
    return m_LoadedAssets.contains(handle);
}

AssetHandle EditorAssetManager::importAsset(const std::filesystem::path &path) {
    AssetHandle handle;
    AssetMetadata metadata;
    metadata.Path = path;
    metadata.Type = getAssetTypeFromFileExtension(path.extension());
    
    std::shared_ptr<AssetBase> asset;
    asset =  AssetImporter(metadata).load();

    if (asset != nullptr) {
        m_LoadedAssets[handle] = asset;
        m_AssetRegistry[handle] = metadata;
        serializeAssetRegistry();
    }
    else {
        std::string fmt = "Failed to load texture at: " + metadata.Path.string();
        Core::Logger::log(Core::LogLevel::Error, fmt.c_str());
    }

    return handle;
}

AssetHandle EditorAssetManager::getAssetHandleFromPath(const std::filesystem::path &path) const {
    for (const auto& [handle, metadata] : m_AssetRegistry) {
        if (metadata.Path == path) {
            return handle;
        }
    }
    return 0;
}

const AssetMetadata& EditorAssetManager::getAssetMetadata(AssetHandle handle) const {
    static AssetMetadata s_NullMetadata;
    auto it = m_AssetRegistry.find(handle);
    if (it != m_AssetRegistry.end()) {
        return it->second;
    }
    else {
        return s_NullMetadata;
    }
}

std::shared_ptr<AssetBase> EditorAssetManager::getAsset(AssetHandle handle) {
    if (!isAssetHandleValid(handle))
        return nullptr;

    std::shared_ptr<AssetBase> asset;
    if (isAssetHandleLoaded(handle)) {
        asset = m_LoadedAssets.at(handle);
    }
    else {
        const AssetMetadata &metadata = getAssetMetadata(handle);

        asset = AssetImporter(metadata).load();

        if(asset == nullptr) {
            std::string fmt = "Failed to load texture from: " + metadata.Path.string();
            Core::Logger::log(Core::LogLevel::Error, fmt.c_str());
        }
        else {
            m_LoadedAssets[handle] = asset;
        }
    }

    return asset;
}

void EditorAssetManager::serializeAssetRegistry() {
    auto path = Project::Project::GetAssetRegistryPath();
    json j;

    for (const auto&[handle, metadata] : m_AssetRegistry) {
        json assetJson;

        assetJson["Handle"] = (uint64_t)handle;
        assetJson["Type"] = assetTypeToString(metadata.Type).c_str();
        assetJson["Path"] = metadata.Path.c_str();

        j["Assets"].push_back(assetJson);
    }

    std::ofstream file(path);
    if (file.is_open()) {
        file << j.dump(4);
    }
}

bool EditorAssetManager::deserializeAssetRegistry() {
    auto path = Project::Project::GetAssetRegistryPath();
    std::ifstream file(path);
    if (!file.is_open())
        return false;

    json j;
    file >> j;

    for (const auto& assetJson : j["Assets"]) {
        AssetMetadata metadata;
        AssetHandle handle = static_cast<AssetHandle>(assetJson["Handle"].get<uint64_t>());
        
        std::string typeStr = assetJson["Type"].get<std::string>();
        if (typeStr == "None") metadata.Type = AssetType::None;
        else if (typeStr == "Texture") metadata.Type = AssetType::Texture;
        else metadata.Type = AssetType::None;
        

        metadata.Path = assetJson["Path"].get<std::filesystem::path>();

        m_AssetRegistry[handle] = metadata;
    }

    return true;
}

}
