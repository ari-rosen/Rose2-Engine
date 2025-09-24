#pragma once

#include "RoseV2/Asset/Asset.hpp"
#include "RoseV2/Project/Project.hpp"

namespace RoseV2::Asset {

template <typename T>
concept AssetDerived = std::is_base_of_v<AssetBase, T>;

class AssetManager {
public:
    template <AssetDerived T>
    static std::shared_ptr<T> getAsset(AssetHandle handle) {
        std::shared_ptr<AssetBase> asset = Project::Project::getActiveProject()->getAssetManager()->getAsset(handle);
        return std::dynamic_pointer_cast<T>(asset);
    }
};

}
