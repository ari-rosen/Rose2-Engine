#include "RoseV2/Core/AssetManager.hpp"
#include "RoseV2/Core/Logger.hpp"
#include "RoseV2/Graphics/Texture.hpp"

namespace RoseV2::Core {

AssetManager& AssetManager::get() {
    static AssetManager instance;
    return instance;
}

AssetManager::AssetManager()
    : m_NextID(1)
{}

uint32_t AssetManager::loadTexture(const std::filesystem::path &path) {
    auto it = m_TexturePathToID.find(path.string());
    if (it != m_TexturePathToID.end()) {
        return it->second;
    }

    std::shared_ptr<Graphics::Texture> texture = Graphics::Texture::create(path);

    uint32_t id = m_NextID++;
    m_Textures[id] = texture;
    m_TexturePathToID[path.string()] = id;
    return id;
}

void AssetManager::unloadTexture(uint32_t id) {
    auto it = m_Textures.find(id);
    if (it != m_Textures.end()) {
        auto pathIt = std::find_if(
            m_TexturePathToID.begin(), m_TexturePathToID.end(),
            [id](const auto &pair) { return pair.second == id; });
        if (pathIt != m_TexturePathToID.end())
            m_TexturePathToID.erase(pathIt);

        m_Textures.erase(it);
    }
}

std::shared_ptr<Graphics::Texture> AssetManager::getTexture(uint32_t id) {
    auto it = m_Textures.find(id);
    if (it != m_Textures.end()) {
        return it->second;
    }
    Logger::log(LogLevel::Error, "Texture ID not found: " + std::to_string(id));
    return nullptr;
}

std::string AssetManager::getTexturePath(uint32_t id) const {
    for (const auto& [path, storedID] : m_TexturePathToID) {
        if (storedID == id)
            return path;
    }
    return {};
}

}
