#include "RoseV2/Scene/Scene.hpp"

namespace RoseV2::Scene {

Scene::Scene()
    : m_IsRunning(false)
{}

void Scene::onStart() {
    m_IsRunning = true;
}

void Scene::onStop() {
    m_IsRunning = false;
}

void Scene::onRuntimeUpdate() {

}

}
