#pragma once

#include "RoseV2/Graphics/Texture.hpp"

namespace RoseV2::Core {

class AssetManager {
public:
    static AssetManager& get();
    AssetManager();

    uint32_t loadTexture(const std::filesystem::path &path);
    void unloadTexture(uint32_t id);
    std::shared_ptr<Graphics::Texture> getTexture(uint32_t id);

    inline uint32_t getLoadedTexturesCount() const { return m_Textures.size(); }
private:
    std::unordered_map<uint32_t, std::shared_ptr<Graphics::Texture>> m_Textures;
    std::unordered_map<std::string, uint32_t> m_TexturePathToID;

    uint32_t m_NextID;
};

}
