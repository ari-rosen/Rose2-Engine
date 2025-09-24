#pragma once

#include "RoseV2/Asset/Asset.hpp"

namespace RoseV2::Graphics {

enum class TextureFormat {
    None = 0,
    RGB8,
    RGBA8
};

struct TextureSpecification {
    uint32_t      Width;
    uint32_t      Height;
    TextureFormat Format;
};

class Texture : public Asset::AssetBase {
public:
    virtual ~Texture() = default;

    static std::shared_ptr<Texture> create(const std::filesystem::path &path);
    static std::shared_ptr<Texture> create(const TextureSpecification &spec);

    virtual void bind(uint32_t slot) const = 0;
    virtual void unbind() const = 0;

    virtual void setData(void *data, uint32_t size) = 0;

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;
    virtual TextureFormat getFormat() const = 0;
    
    virtual uint32_t getRendererID() const = 0;

    static Asset::AssetType getStaticAssetType() { return Asset::AssetType::Texture; }
    Asset::AssetType getAssetType() const override { return getStaticAssetType(); }
};

}
