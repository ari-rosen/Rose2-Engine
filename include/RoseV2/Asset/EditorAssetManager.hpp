#pragma once

#include "RoseV2/Asset/Asset.hpp"
#include "RoseV2/Asset/AssetManagerBase.hpp"

namespace RoseV2::Asset {

using AssetMap = std::map<AssetHandle, std::shared_ptr<AssetBase>>;
using AssetRegistry = std::map<AssetHandle, AssetMetadata>;

class EditorAssetManager : public AssetManagerBase {
public:
    AssetHandle importAsset(const std::filesystem::path &path);
    AssetHandle getAssetHandleFromPath(const std::filesystem::path &path) const;
    std::shared_ptr<AssetBase> getAsset(AssetHandle handle) override;
    const AssetMetadata& getAssetMetadata(AssetHandle handle) const;

    void serializeAssetRegistry();
    bool deserializeAssetRegistry();
private:
    bool isAssetHandleValid(AssetHandle handle) const;
    bool isAssetHandleLoaded(AssetHandle handle) const;
private:
    AssetMap m_LoadedAssets;
    AssetRegistry m_AssetRegistry;
};

}
