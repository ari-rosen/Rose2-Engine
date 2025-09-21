#pragma once

namespace RoseV2::Core {

class Timestep {
public:
    Timestep(float time = 0.0f) : m_Time(time) {}

    float getSeconds() const { return m_Time; }
    float getMiliseconds() const { return m_Time * 1000; }

    operator float() const { return m_Time; }
private:
    float m_Time;
};

}
