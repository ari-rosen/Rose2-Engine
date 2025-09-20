#pragma once

namespace RoseV2::Graphics {

enum class TextureFormat {
    None = 0,
    RGB8,
    RGBA8
};

class Texture {
public:
    virtual ~Texture() = default;

    static std::shared_ptr<Texture> create(const std::filesystem::path &path);
    static std::shared_ptr<Texture> create();

    virtual void bind(uint32_t slot) const = 0;
    virtual void unbind() const = 0;

    virtual void setData(void *data, uint32_t size) = 0;

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;
    virtual TextureFormat getFormat() const = 0;
    
    virtual uint32_t getRendererID() const = 0;
};

}
