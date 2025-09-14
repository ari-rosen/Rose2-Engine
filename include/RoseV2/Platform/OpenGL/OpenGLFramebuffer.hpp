#pragma once

#include "RoseV2/Graphics/Framebuffer.hpp"

namespace RoseV2::Platform::OpenGL {

class OpenGLFramebuffer : public Graphics::Framebuffer {
public:
    OpenGLFramebuffer(const Graphics::FramebufferSpecification& spec);
    ~OpenGLFramebuffer() override;

    void bind() const override;
    void unbind() const override;

    void resize(uint32_t width, uint32_t height) override;
    int readPixel(uint32_t attachmentIndex, int x, int y) override;
    void clearAttachment(uint32_t attachmentIndex, int value) override;

    uint32_t getColorAttachmentRendererID(uint32_t index) const override;
    const Graphics::FramebufferSpecification& getSpecification() const override;

private:
    void invalidate();
private:
    uint32_t m_RendererID = 0;
    std::vector<uint32_t> m_ColorAttachments;
    uint32_t m_DepthAttachment = 0;
    Graphics::FramebufferSpecification m_Spec;
};

}

