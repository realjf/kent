#include <kent/platform/application.h>

namespace Kent {

    Application::Application(const std::string& title)
    {
        m_Window = Window::CreateWindow(WindowProps(title));
    }

    void Application::Run() 
    {
        while(m_Running)
        {
            if(!m_Window->OnUpdate())
            {
                m_Running = false;
            }
        }
        
    }

    void Application::Close() 
    {
        m_Running = false;
    }
}
