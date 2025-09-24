#pragma once

#include "RoseV2/Asset/Asset.hpp"

namespace RoseV2::Asset {

class AssetManagerBase {
public:
    virtual ~AssetManagerBase() = default;
    
    virtual std::shared_ptr<AssetBase> getAsset(AssetHandle handle) = 0;
};

}
