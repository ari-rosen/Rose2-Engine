#include "RoseV2/Graphics/Renderer.hpp"
#include "RoseV2/Graphics/OpenGLRenderer.hpp"

namespace RoseV2::Graphics {

std::unique_ptr<Renderer> Renderer::create() {
    return std::make_unique<OpenGLRenderer>(); 
}

}
