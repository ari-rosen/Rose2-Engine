#pragma once

#include "RoseV2/Graphics/Texture.hpp"

namespace RoseV2::Platform::OpenGL {

class OpenGLTexture : public Graphics::Texture {
public:    
    OpenGLTexture(const std::filesystem::path &path);
    OpenGLTexture(Graphics::TextureFormat format);

    ~OpenGLTexture();

    void bind(uint32_t slot) const override;
    void unbind() const override;

    void setData(void *data, uint32_t size) override;

    uint32_t getWidth() const override;
    uint32_t getHeight() const override;
    Graphics::TextureFormat getFormat() const override;
private:
    uint32_t m_RendererID;
    uint32_t m_Width;
    uint32_t m_Height;
    Graphics::TextureFormat m_Format;

};

}
