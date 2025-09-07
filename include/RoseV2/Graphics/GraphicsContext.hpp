#pragma once

namespace RoseV2::Graphics {

class GraphicsContext {
public:
    virtual ~GraphicsContext() = default;

    static std::unique_ptr<GraphicsContext> create();

    virtual void init() = 0;
};

} // namespace RoseV2::Graphics
