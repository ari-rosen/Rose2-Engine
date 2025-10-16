#include "RoseV2/Asset/AssetImporter.hpp"
#include "RoseV2/Asset/Asset.hpp"
#include "RoseV2/Core/Logger.hpp"
#include "RoseV2/Graphics/Texture.hpp"
#include "RoseV2/Script/LuaEngine.hpp"
#include "RoseV2/Script/ScriptInstance.hpp"
#include "RoseV2/Script/LuaHeaders.hpp"

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
    if (!std::filesystem::exists(m_Metadata.Path)) {
        Core::Logger::log(Core::LogLevel::Error, "Script file not found: " + m_Metadata.Path.string());
        return nullptr;
    }

    lua_State *L = Script::LuaEngine::getLuaState();

    if (luaL_loadfile(L, m_Metadata.Path.string().c_str()) != LUA_OK) {
        Core::Logger::log(Core::LogLevel::Error, 
                          std::string{"Lua compilation failed for "} + m_Metadata.Path.stem().string() + ": " + lua_tostring(L, -1));
        lua_pop(L, 1);
        return nullptr;
    }

    std::vector<std::byte> bytecodeBuffer;

    if (lua_dump(L, luaBytecodeWriterWrapper, &bytecodeBuffer, false) != 0) {
        Core::Logger::log(Core::LogLevel::Error, "Failed to dump Lua bytecode to memory.");
        lua_pop(L, 1);
        return nullptr;
    }

    lua_pop(L, 1); 

    std::string scriptName = m_Metadata.Path.stem().string();
    auto script = std::make_shared<Script::ScriptInstance>(scriptName);
    
    if (!script->loadSource(bytecodeBuffer)) {
        Core::Logger::log(Core::LogLevel::Error, 
                          std::string{"Failed to load bytecode into script instance: "} + scriptName);
        return nullptr;
    }

    return script;
}

}
