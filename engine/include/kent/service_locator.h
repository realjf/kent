#pragma once
#include <memory>
#include <kent/platform/window.h>

namespace Kent {
    class ServiceLocator {
    public:
        static inline const std::unique_ptr<Window>& GetWindow() { return m_Window; }
        static inline void Provide(Window* window)
        { 
            if(m_Window != nullptr) return;
            m_Window = std::unique_ptr<Window>(window);
        }
    private:
        static inline std::unique_ptr<Window> m_Window = nullptr;
    };

}