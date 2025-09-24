#include "RoseV2/Platform/OpenGL/OpenGLTexture.hpp"
#include "RoseV2/Graphics/Texture.hpp"
#include "RoseV2/Platform/OpenGL/OpenGLError.hpp"
#include "RoseV2/Core/Assert.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

namespace RoseV2::Platform::OpenGL {

static void getGLFormat(Graphics::TextureFormat format, GLenum& internalFormat, GLenum& dataFormat) {
    switch (format) {
        case Graphics::TextureFormat::RGB8:
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
            break;
        case Graphics::TextureFormat::RGBA8:
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
            break;
        default:
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
            break;
    }
}

OpenGLTexture::OpenGLTexture(const std::filesystem::path &path) {
    int width, height, channels;
    stbi_uc *data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

    std::string assertMsg = "Failed to load texture from: " + path.string();
    ROSEV2_ASSERT(data, assertMsg.c_str());

    m_Width = width;
    m_Height = height;
    
    if (channels == 4) m_Format = Graphics::TextureFormat::RGBA8;
    else if (channels == 3) m_Format = Graphics::TextureFormat::RGB8;
    else m_Format = Graphics::TextureFormat::RGBA8;

    GLenum internalFormat, dataFormat;
    getGLFormat(m_Format, internalFormat, dataFormat);

    GL_CALL(glGenTextures(1, &m_RendererID));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, data));
    GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

    stbi_image_free(data);
}

OpenGLTexture::OpenGLTexture(const Graphics::TextureSpecification &spec)
    : m_RendererID(0), m_Width(spec.Width), m_Height(spec.Height), m_Format(spec.Format) 
{
    GLenum internalFormat, dataFormat;
    getGLFormat(m_Format, internalFormat, dataFormat);

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, nullptr);
}

OpenGLTexture::~OpenGLTexture() {
    glDeleteTextures(1, &m_RendererID);
}

void OpenGLTexture::bind(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void OpenGLTexture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLTexture::setData(void *data, uint32_t size) {
    uint32_t bytesPerPixel = (m_Format == Graphics::TextureFormat::RGBA8) ? 4 : 3;
    ROSEV2_ASSERT(size == m_Width * m_Height * bytesPerPixel, "Attempt to assign texture data that does not match texture dimensions");

    GLenum internalFormat, dataFormat;
    getGLFormat(m_Format, internalFormat, dataFormat);

    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0, 0,
        m_Width,
        m_Height,
        dataFormat,
        GL_UNSIGNED_BYTE,
        data
    );
}

uint32_t OpenGLTexture::getWidth() const {
    return m_Width;
}

uint32_t OpenGLTexture::getHeight() const {
    return m_Height;
}

Graphics::TextureFormat OpenGLTexture::getFormat() const {
    return m_Format;
}


}
