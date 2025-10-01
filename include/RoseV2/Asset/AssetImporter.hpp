#pragma once

#include "RoseV2/Asset/Asset.hpp"

namespace RoseV2::Asset {

class AssetImporter {
public:
    AssetImporter(const AssetMetadata &metadata);

    std::shared_ptr<AssetBase> load();
    
    template <AssetDerived T>
    std::shared_ptr<T> loadAs() {
        std::shared_ptr<AssetBase> asset;

        switch (m_Metadata.Type) {
            case AssetType::None:
                asset = nullptr;
                break;
            case AssetType::Texture:
                asset = loadTexture();
                break;
            default:
                asset = nullptr;
                break;
        }

        return std::static_pointer_cast<T>(asset);
    }
private:
    std::shared_ptr<AssetBase> loadTexture(); 
private:
    AssetMetadata m_Metadata;
};

}
