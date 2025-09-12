#include "RoseV2/Graphics/Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace RoseV2::Graphics {

Camera::Camera(float left, float right, float bottom, float top)
    : m_Position(0.0f), m_Rotation(0.0f), m_Zoom(1.0f) {
    setProjection(left, right, bottom, top);
}

void Camera::setProjection(float left, float right, float bottom, float top) {
    m_Projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    recalculateViewProjection();
}

void Camera::setPosition(const glm::vec2 &position) {
    m_Position = position;
    recalculateViewProjection();
}

void Camera::setRotation(float rotation) {
    m_Rotation = rotation;
    recalculateViewProjection();
}

void Camera::setZoom(float zoom) {
    m_Zoom = zoom;
    recalculateViewProjection();
}

void Camera::recalculateViewProjection() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, 0.0f))
                        * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0,0,1))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(m_Zoom, m_Zoom, 1.0f));

    m_View = glm::inverse(transform);
    m_ViewProjection = m_Projection * m_View;
}

}
