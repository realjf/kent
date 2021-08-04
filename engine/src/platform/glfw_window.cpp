#include "glfw_window.h"
#include <iostream>

namespace Kent {

    CustomWindow::CustomWindow()
        : m_Window(nullptr)
    {

    }

    static void error_callback(int error, const char* description)
    {
        std::cerr << "Error: " << description << std::endl;
    }

    void CustomWindow::CreateWindow()
    {
        
        glfwSetErrorCallback(error_callback);
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        m_Window = glfwCreateWindow(m_Width, m_Height, "Kent Engine", nullptr, nullptr);

        glfwMakeContextCurrent(m_Window);


    }

    bool CustomWindow::Run()
    {
        while(!glfwWindowShouldClose(m_Window))
        {
            glfwPollEvents();

            glfwSwapBuffers(m_Window);
        }

        glfwTerminate();
        return 0;
    }
}
