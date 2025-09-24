#pragma once

#include "RoseV2/Asset/Asset.hpp"

namespace RoseV2::Asset {

class AssetImporter {
public:
    AssetImporter(const AssetMetadata &metadata);

    std::shared_ptr<AssetBase> load();
private:
    std::shared_ptr<AssetBase> loadTexture(); 
private:
    AssetMetadata m_Metadata;
};

}
