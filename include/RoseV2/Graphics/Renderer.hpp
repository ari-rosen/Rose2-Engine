#pragma once

#include "RoseV2/Core/Core.hpp"
#include "RoseV2/Graphics/Shader.hpp"
#include "RoseV2/Graphics/VertexArray.hpp"

namespace RoseV2::Graphics {

class ROSE_V2_API Renderer {
public:
    static void init();
    static void beginScene();
    static void submitVertexArray(const std::shared_ptr<VertexArray> &vertexArray,
                                  const std::shared_ptr<Shader> &shader);
    static void endScene();
private:
    struct DrawCall {
        std::shared_ptr<VertexArray> VertexArray;
        std::shared_ptr<Shader>      Shader;
    };
private:
    static std::vector<DrawCall> s_DrawQueue;
};

} // RoseV2::Graphics
