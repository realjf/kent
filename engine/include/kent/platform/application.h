#pragma once

#include <kent/platform/reference.h>
#include <kent/platform/window.h>
#include <string>



namespace Kent {

    class Application {
    public:
        Application(const std::string& title = "Kent App");
        virtual ~Application() = default;

        void Run();
        void Close();

        inline Window& GetWindow() { return *m_Window; }

    private:

    private:
        std::string m_Title;
        Scope<Window> m_Window;

        bool m_Running = true;
    };

    extern Application* CreateApplication();
}