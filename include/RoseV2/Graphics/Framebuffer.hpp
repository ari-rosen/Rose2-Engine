#pragma once

namespace RoseV2::Graphics {

enum class FramebufferTextureFormat {
	None = 0,
	RGBA8,
	RED_INTEGER,
	DEPTH24STENCIL8,
	Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification {
	FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
};

struct FramebufferSpecification {
    uint32_t Width = 0;
    uint32_t Height = 0;
    uint32_t Samples = 1;
    bool SwapChainTarget = false;
    std::vector<FramebufferTextureSpecification> Attachments;
};

class Framebuffer {
public:
    virtual ~Framebuffer() = default;

    static std::shared_ptr<Framebuffer> create(const FramebufferSpecification &spec);

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void resize(uint32_t width, uint32_t height) = 0;
    virtual int readPixel(uint32_t attachmentIndex, int x, int y) = 0;
    virtual void clearAttachment(uint32_t attachmentIndex, int value) = 0;

    virtual uint32_t getColorAttachmentRendererID(uint32_t index = 0) const = 0;
    virtual const FramebufferSpecification& getSpecification() const = 0;
};

}

