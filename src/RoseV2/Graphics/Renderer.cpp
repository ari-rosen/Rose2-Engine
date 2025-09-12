#include "RoseV2/Graphics/Renderer.hpp"

#include "RoseV2/Graphics/Buffer.hpp"
#include "RoseV2/Graphics/Camera.hpp"
#include "RoseV2/Graphics/RenderCommand.hpp"
#include "RoseV2/Graphics/UniformBuffer.hpp"
#include "RoseV2/Graphics/Shader.hpp"
#include "RoseV2/Graphics/VertexArray.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

namespace RoseV2::Graphics {

struct Vertex {
    glm::vec3 Position;
    glm::vec4 Color;
};

struct RendererData {
    static const uint32_t MaxQuads = 30000;
	static const uint32_t MaxVertices = MaxQuads * 4;
	static const uint32_t MaxIndices = MaxQuads * 6;

    std::shared_ptr<VertexArray> QuadVertexArray;
    std::shared_ptr<VertexBuffer> QuadVertexBuffer;
    std::shared_ptr<Shader> QuadShader;

    uint32_t QuadIndexCount = 0;
	Vertex* QuadVertexBufferBase = nullptr;
    Vertex* QuadVertexBufferPtr = nullptr;

	glm::vec4 QuadVertexPositions[4];
    
    struct CameraData {
        glm::mat4 ViewProjMatrix;
    };
    CameraData CameraBufferData;
    std::shared_ptr<UniformBuffer> CameraUniformBuffer;
};

static RendererData s_Data;

void Renderer::init() {
    RenderCommand::init();
    
    s_Data.QuadShader = Shader::create(
        "src/RoseV2/Graphics/CoreShaders/PlainColor.vert",
        "src/RoseV2/Graphics/CoreShaders/PlainColor.frag"
    );

    s_Data.QuadVertexBufferBase = new Vertex[s_Data.MaxVertices];

    s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
    s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
    s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

	s_Data.QuadVertexArray = VertexArray::create();
    
    s_Data.QuadVertexBuffer = VertexBuffer::create(s_Data.MaxVertices * sizeof(Vertex));
    s_Data.QuadVertexBuffer->setBufferLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4, "a_Color" }
    });
    s_Data.QuadVertexArray->addVertexBuffer(s_Data.QuadVertexBuffer);

	uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

	uint32_t offset = 0;
	for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}

    std::shared_ptr<IndexBuffer> quadIB = IndexBuffer::create(quadIndices, s_Data.MaxIndices);
    s_Data.QuadVertexArray->setIndexBuffer(quadIB);

    delete[] quadIndices;

    s_Data.QuadIndexCount = 0;
    
    s_Data.CameraUniformBuffer = UniformBuffer::create(sizeof(RendererData::CameraData), 0);
    s_Data.QuadShader->setUniformBinding("Camera", 0);
}

void Renderer::shutdown() {
    delete[] s_Data.QuadVertexBufferBase;

    s_Data.QuadVertexArray.reset();
    s_Data.QuadVertexBuffer.reset();
    s_Data.QuadShader.reset();
}

void Renderer::beginBatch() {
    s_Data.QuadIndexCount = 0;
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
}

void Renderer::flushBatch() {
    if (s_Data.QuadIndexCount == 0)
        return;

    uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
	s_Data.QuadVertexBuffer->setData(s_Data.QuadVertexBufferBase, dataSize);

	s_Data.QuadShader->bind();
	RenderCommand::drawIndexed(s_Data.QuadVertexArray);
}

void Renderer::beginScene(const Camera &camera) {
    RenderCommand::clear();
    RenderCommand::setClearColor(0.2, 0.2, 0.2);

    s_Data.CameraBufferData.ViewProjMatrix = camera.getViewProjectionMatrix();
    s_Data.CameraUniformBuffer->setData(&s_Data.CameraBufferData, sizeof(RendererData::CameraData));
    
    beginBatch();
}

void Renderer::submitQuad(const glm::mat4 &transform, const glm::vec4 &color) {
    constexpr size_t quadVertexCount = 4;

    if (s_Data.QuadIndexCount >= RendererData::MaxIndices) {
        flushBatch();
        beginBatch();
    }
    
    for (size_t i = 0; i < quadVertexCount; i++) {
        s_Data.QuadVertexBufferPtr->Position = glm::vec3(transform * s_Data.QuadVertexPositions[i]);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr++;
	}

    s_Data.QuadIndexCount += 6;
}

void Renderer::submitSprite(const ECS::TransformComponent &transform, const ECS::SpriteComponent &sprite) {
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), transform.Translation);

    glm::mat4 rotation = glm::yawPitchRoll(
        transform.Rotation.y,
        transform.Rotation.x,
        transform.Rotation.z
    );

    glm::mat4 scale = glm::scale(glm::mat4(1.0f), transform.Scale);

    glm::mat4 finalTransform = translation * rotation * scale;

    submitQuad(finalTransform, sprite.Color);
}

void Renderer::endScene() {
    flushBatch();
}


}
