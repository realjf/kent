#include "windowswindow.h"
#include <iostream>

namespace Kent
{

    CustomWindow::CustomWindow(const WindowProps &props)
    {
        Init(props);
    }

    CustomWindow::~CustomWindow()
    {
        Shutdown();
    }

    Scope<Window> Window::CreateWindow(const WindowProps &props)
    {
        return CreateScope<CustomWindow>(props);
    }

    static void error_callback(int error, const char *description)
    {
        std::cerr << "GLFW Error: " << description << std::endl;
    }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    void CustomWindow::Init(const WindowProps &props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        glfwSetErrorCallback(error_callback);
        glfwInit();

        // vulkan require
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
        if(!m_Window)
        {
            glfwTerminate();
            exit(-1);
        }

        glfwSetKeyCallback(m_Window, key_callback);

        glfwMakeContextCurrent(m_Window);
    }

    bool CustomWindow::OnUpdate()
    {
        if (glfwWindowShouldClose(m_Window))
        {
            return false;
        }

        glfwPollEvents();

        // glfwSwapBuffers(m_Window);

        return true;
    }
    
    std::pair<int, int> CustomWindow::GetWindowExtents() 
    {
        int width, height;
        glfwGetFramebufferSize(m_Window, &width, &height);
        return {width, height};
    }

    void CustomWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
        
        glfwTerminate();
    }
}
