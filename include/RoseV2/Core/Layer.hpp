#pragma once

#include "RoseV2/Core/Timestep.hpp"

namespace RoseV2::Core {

class Application;

class Layer {
public:
    virtual ~Layer() = default;

    virtual void onUpdate(Timestep deltaTime) = 0;
    virtual void onRender() = 0;
protected:
    Application *m_Application;
};

}
