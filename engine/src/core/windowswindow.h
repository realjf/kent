#pragma once

#define GLFW_INCLUDE_VULKAN

#include <kent/core/window.h>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>


namespace Kent {

    class CustomWindow : public Window {
    public:
        CustomWindow(const WindowProps& props);
        virtual ~CustomWindow();
        
        virtual bool OnUpdate();
        

        inline virtual uint32_t GetWidth() const override { return m_Data.Width; }
        inline virtual uint32_t GetHeight() const override { return m_Data.Height; }

        virtual std::pair<int, int> GetWindowExtents() override;
        
        inline virtual GLFWwindow* GetWindow() override { return m_Window; }

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

