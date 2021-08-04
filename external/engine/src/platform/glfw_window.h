#pragma once

#include <kent_engine/platform/window.h>
#include <GLFW/glfw3.h>

namespace Kent {

    class CustomWindow : public Window {
    public:
        CustomWindow();
        virtual ~CustomWindow() = default;
        virtual void CreateWindow() override;
        virtual bool Run() override;
        

    private:
        GLFWwindow* m_Window;
        uint32_t m_Width = 1600, m_Height = 900;
    };
}

