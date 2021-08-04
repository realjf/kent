#pragma once

#include <string>

namespace Kent {

    class Application {
    public:
        Application() = default;
        Application(std::string title);
        virtual ~Application() = default;

        void Run();
    private:
        void Init();
        void Shutdown();

    private:
        std::string m_Title;
    };

    extern Application* CreateApplication();
}