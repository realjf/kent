#include "windowswindow.h"
#include <iostream>

namespace Kent {

    CustomWindow::CustomWindow(const WindowProps& props)
    {
        Init(props);
    }

    CustomWindow::~CustomWindow()
    {
        Shutdown();
    }

    Scope<Window> Window::CreateWindow(const WindowProps& props)
    {
        return CreateScope<CustomWindow>(props);
    }

    static void error_callback(int error, const char* description)
    {
        std::cerr << "Error: " << description << std::endl;
    }

    void CustomWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        glfwSetErrorCallback(error_callback);
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);

        glfwMakeContextCurrent(m_Window);
    }

    bool CustomWindow::OnUpdate()
    {
        if(glfwWindowShouldClose(m_Window))
        {
             return false;
        }
        
        glfwPollEvents();

        glfwSwapBuffers(m_Window);

        return true;
    }

    void CustomWindow::Shutdown()
    {
        glfwTerminate();
    }
}
