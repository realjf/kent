#pragma once

#include <kent/core/reference.h>
#include <GLFW/glfw3.h>

#include <string>
#include <tuple>


namespace Kent {

    
    
    struct WindowProps
    {
        uint32_t Width, Height;
        std::string Title;

        WindowProps(const std::string& title, uint32_t width = 1600, uint32_t height = 900)
        : Title(title), Width(width), Height(height)
        {

        }
    };
    class Window 
    {
    public:
        virtual ~Window() {};
        virtual bool OnUpdate() = 0;

        static Scope<Window> CreateWindow(const WindowProps& props);
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual GLFWwindow* GetWindow() = 0;

        virtual std::pair<int, int> GetWindowExtents() = 0;
        
    private:
        
    };
}