#pragma once

#include <kent/platform/window.h>
#include <GLFW/glfw3.h>

namespace Kent {

    class CustomWindow : public Window {
    public:
        CustomWindow(const WindowProps& props);
        virtual ~CustomWindow();
        
        virtual bool OnUpdate();
        

        inline virtual uint32_t GetWidth() const override { return m_Data.Width; }
        inline virtual uint32_t GetHeight() const override { return m_Data.Height; }
    private:
        virtual void Shutdown();
        virtual void Init(const WindowProps& props);


    private:
        GLFWwindow* m_Window;

        struct WindowData {
            std::string Title;
            uint32_t Width, Height;
        };

        WindowData m_Data;
    };
}

