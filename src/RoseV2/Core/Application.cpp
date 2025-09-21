#include "RoseV2/Core/Application.hpp"
#include "RoseV2/Core/Logger.hpp"
#include "RoseV2/Graphics/Renderer.hpp"

namespace RoseV2::Core {

Application::Application() {
    init("RoseV2 Application", 1080, 680); 
}

Application::Application(const std::string& title, uint32_t width, uint32_t height) {
    init(title, width, height);
}

Application::~Application() {
    Logger::log(LogLevel::Info, "Application shutting down");
    Graphics::Renderer::shutdown();
}


void Application::init(const std::string& title, uint32_t width, uint32_t height) {
#ifdef PROJECT_SOURCE_DIR
    std::filesystem::current_path(PROJECT_SOURCE_DIR);
#endif
    Logger::log(LogLevel::Info, "Application initalizing");
    
    Logger::log(LogLevel::Info, "Creating window");
    m_Window = Window::create(title, width, height);

    Graphics::Renderer::init();

    m_Running = true;
}

void Application::pushLayer(std::unique_ptr<Layer> layer) {
    m_LayerStack.push_back(std::move(layer));
}

Timestep Application::getDeltaTime() {
    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta = currentTime - lastTime; // convert to seconds as float
    lastTime = currentTime;
    return Timestep(delta.count());
}

void Application::run() {
    Logger::log(LogLevel::Info, "Application started");

    while (m_Running) {
        m_Window->pollEvents();

        for (auto &layer : m_LayerStack) {
            layer->onUpdate(getDeltaTime());
        }

        for (auto &layer : m_LayerStack) {
            layer->onRender();
        }

        m_Window->swapBuffers();

        if (m_Window->shouldClose())
            stop();
    }
}

void Application::stop() {
    m_Running = false;
}

} // namespace RoseV2::Core
