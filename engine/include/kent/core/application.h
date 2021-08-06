#pragma once

#include <kent/core/reference.h>
#include <kent/core/window.h>
#include <kent/renderer/renderer.h>
#include <string>



namespace Kent {

    class Application {
    public:
        Application(const std::string& title = "Kent App");
        virtual ~Application() = default;

        void Run();
        void Close();

        inline Window& GetWindow() { return *m_Window; }

        inline Renderer& GetRenderer() { return *m_Renderer; }

    private:

    private:
        std::string m_Title;
        Scope<Window> m_Window;
        Scope<Renderer> m_Renderer;

        bool m_Running = true;
    };

    extern Application* CreateApplication();
}