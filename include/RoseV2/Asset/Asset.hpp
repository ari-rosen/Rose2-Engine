#pragma once

#include "RoseV2/Core/UUID.hpp"

namespace RoseV2::Asset {

using AssetHandle = Core::UUID;

enum class AssetType : uint16_t {
    None = 0,
    Texture
};

struct AssetMetadata {
    AssetType Type = AssetType::None;
    std::filesystem::path Path;

    bool isNull() const { return Type == AssetType::None; }
};

class AssetBase {
public:
    AssetHandle Handle = 0;

    virtual AssetType getAssetType() const = 0;
};

}
