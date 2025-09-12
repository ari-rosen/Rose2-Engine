#pragma once

#include <glm/glm.hpp>

namespace RoseV2::Graphics {

class Camera {
public:
    Camera(float left, float right, float bottom, float top);

    void setProjection(float left, float right, float bottom, float top);
    
    void setPosition(const glm::vec2 &position);
    inline const glm::vec2& getPosition() const { return m_Position; };

    void setRotation(float rotation);
    inline float getRotation() const { return m_Rotation; };

    void setZoom(float zoom);
    inline float getZoom() const { return m_Zoom; };

    inline const glm::mat4& getViewMatrix() const { return m_View; }
    inline const glm::mat4& getProjectionMatrix() const { return m_Projection; }
    inline const glm::mat4& getViewProjectionMatrix() const { return m_ViewProjection; }
private:
    void recalculateViewProjection();
private:
    glm::vec2 m_Position;
    float m_Rotation;
    float m_Zoom;

    glm::mat4 m_View{1.0f};
    glm::mat4 m_Projection{1.0f};
    glm::mat4 m_ViewProjection{1.0f};
};

}
