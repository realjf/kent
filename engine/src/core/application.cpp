#include <kent/core/application.h>

namespace Kent {

    Application::Application(const std::string& title)
        : m_Title(title)
    {
        m_Window = Window::CreateWindow(WindowProps(title));
        m_Renderer = Renderer::CreateRenderer();
    }

    void Application::Run() 
    {
        RendererSettings settings = {
            .ApplicationName = m_Title,
        };
        auto [width, height] = m_Window->GetWindowExtents();
        m_Renderer->Init(settings, m_Window->GetWindow(), width, height);
        

        while(m_Running)
        {
            if(!m_Window->OnUpdate())
            {
                m_Renderer->Shutdown();
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
        m_Renderer.reset();
        m_Renderer = nullptr;
        m_Running = false;
    }
}
