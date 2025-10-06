#include "RoseV2/Asset/AssetImporter.hpp"
#include "RoseV2/Asset/Asset.hpp"
#include "RoseV2/Core/Logger.hpp"
#include "RoseV2/Graphics/Texture.hpp"
#include "RoseV2/Script/ScriptInstance.hpp"

#include <stb_image.h>

namespace RoseV2::Asset {

AssetImporter::AssetImporter(const AssetMetadata &metadata)
    : m_Metadata(metadata)
{}

std::shared_ptr<AssetBase> AssetImporter::load() {
    switch(m_Metadata.Type) {
        case AssetType::None: return nullptr;
        case AssetType::Texture: return loadTexture();
        case AssetType::Script: return loadScript();
        default: return nullptr;
    }
}

std::shared_ptr<AssetBase> AssetImporter::loadTexture() {
    int width, height, channels;
    stbi_uc* data = nullptr;
    {
        data = stbi_load(m_Metadata.Path.c_str(), &width, &height, &channels, 0);
    }
    if (!data) 
        return nullptr;
    
    size_t size = static_cast<size_t>(width) * height * channels;

    Graphics::TextureSpecification spec;
    spec.Height = height;
    spec.Width = width;
    switch (channels) {
        case 3: spec.Format = Graphics::TextureFormat::RGB8; break;
        case 4: spec.Format = Graphics::TextureFormat::RGBA8; break;
        default: spec.Format = Graphics::TextureFormat::None; break;
    }
    
    std::shared_ptr<Graphics::Texture> texture = Graphics::Texture::create(spec);
    texture->setData(data, size); 

    stbi_image_free(data);

    return texture;
}

std::shared_ptr<AssetBase> AssetImporter::loadScript() {
    std::ifstream file(m_Metadata.Path);
    if (!file.is_open()) {
        return nullptr;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string source = ss.str();
    file.close();

    auto script = std::make_shared<Script::ScriptInstance>(m_Metadata.Path.stem());
    
    if (!script->loadSource(source)) {
        Core::Logger::log(Core::LogLevel::Error, std::string{"Failed to load source code for script: "} + m_Metadata.Path.stem().string());
        return nullptr;
    }

    return script;
}

}
