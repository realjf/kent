#pragma once

namespace Kent {
    class Window 
    {
    public:
        virtual ~Window() {};
        virtual void CreateWindow() = 0;
        virtual bool Run() = 0;
    };
}