#include "RoseV2/Platform/OpenGL/OpenGLFramebuffer.hpp"
#include "RoseV2/Graphics/Framebuffer.hpp"

#include <glad/glad.h>
#include <iostream>

namespace RoseV2::Platform::OpenGL {

static GLenum framebufferTextureTarget(bool multisampled) {
    return GL_TEXTURE_2D;
}

static GLenum textureFormatToGL(Graphics::FramebufferTextureFormat format) {
    switch (format) {
        case Graphics::FramebufferTextureFormat::RGBA8: return GL_RGBA8;
        case Graphics::FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
        case Graphics::FramebufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
        case Graphics::FramebufferTextureFormat::None: break;
    }
    return 0;
}

OpenGLFramebuffer::OpenGLFramebuffer(const Graphics::FramebufferSpecification& spec)
    : m_Spec(spec)
{
    invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
    if (m_ColorAttachments.size())
        glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
    if (m_DepthAttachment)
        glDeleteTextures(1, &m_DepthAttachment);
    if (m_RendererID)
        glDeleteFramebuffers(1, &m_RendererID);
}

void OpenGLFramebuffer::invalidate() {
    if (m_RendererID) {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);
        m_ColorAttachments.clear();
        m_DepthAttachment = 0;
    }

    glGenFramebuffers(1, &m_RendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    bool hasDepth = false;

    m_ColorAttachments.resize(m_Spec.Attachments.size());
    for (size_t i = 0; i < m_Spec.Attachments.size(); i++) {
        const auto& spec = m_Spec.Attachments[i];
        GLenum format = textureFormatToGL(spec.TextureFormat);
        if (spec.TextureFormat == Graphics::FramebufferTextureFormat::DEPTH24STENCIL8) {
            hasDepth = true;
            continue;
        }

        glGenTextures(1, &m_ColorAttachments[i]);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_Spec.Width, m_Spec.Height, 0,
                     spec.TextureFormat == Graphics::FramebufferTextureFormat::RED_INTEGER ? GL_RED_INTEGER : GL_RGBA,
                     GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (GLenum)i, GL_TEXTURE_2D, m_ColorAttachments[i], 0);
    }

    if (hasDepth) {
        glGenTextures(1, &m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Spec.Width, m_Spec.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
    }

    if (m_ColorAttachments.size() > 1) {
        GLenum buffers[16];
        for (size_t i = 0; i < m_ColorAttachments.size(); i++)
            buffers[i] = GL_COLOR_ATTACHMENT0 + (GLenum)i;
        glDrawBuffers((GLsizei)m_ColorAttachments.size(), buffers);
    } else if (m_ColorAttachments.empty()) {
        glDrawBuffer(GL_NONE);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "OpenGLFramebuffer: Framebuffer is incomplete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glViewport(0, 0, m_Spec.Width, m_Spec.Height);
}

void OpenGLFramebuffer::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::resize(uint32_t width, uint32_t height) {
    m_Spec.Width = width;
    m_Spec.Height = height;
    invalidate();
}

int OpenGLFramebuffer::readPixel(uint32_t attachmentIndex, int x, int y) {
    if (attachmentIndex >= m_ColorAttachments.size()) return -1;
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    int pixelData = 0;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return pixelData;
}

void OpenGLFramebuffer::clearAttachment(uint32_t attachmentIndex, int value) {
    if (attachmentIndex >= m_ColorAttachments.size()) return;
    auto spec = m_Spec.Attachments[attachmentIndex];
    GLenum format = textureFormatToGL(spec.TextureFormat);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    if (spec.TextureFormat == Graphics::FramebufferTextureFormat::RED_INTEGER) {
        glClearBufferiv(GL_COLOR, attachmentIndex, &value);
    } else {
        float clearColor[4] = {0,0,0,0};
        glClearBufferfv(GL_COLOR, attachmentIndex, clearColor);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

uint32_t OpenGLFramebuffer::getColorAttachmentRendererID(uint32_t index) const {
    if (index >= m_ColorAttachments.size()) return 0;
    return m_ColorAttachments[index];
}

const Graphics::FramebufferSpecification& OpenGLFramebuffer::getSpecification() const {
    return m_Spec;
}

} // namespace RoseV2::Platform::OpenGL
