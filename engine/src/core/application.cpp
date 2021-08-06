#include <kent/core/application.h>

namespace Kent {

    Application::Application(const std::string& title)
    {
        m_Window = Window::CreateWindow(WindowProps(title));
        m_Renderer = Renderer::CreateRenderer();
    }

    void Application::Run() 
    {
        // m_Renderer->Init();

        while(m_Running)
        {
            if(!m_Window->OnUpdate())
            {
                Close();
            }

            // Draw
            m_Renderer->RenderFrame();
        }
        
    }

    void Application::Close() 
    {
        m_Window.reset();
        m_Window = nullptr;
        m_Running = false;
    }
}
