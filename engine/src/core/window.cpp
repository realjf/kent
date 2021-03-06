#include <kent/core/window.h>
#include "windowswindow.h"

namespace Kent {
    Scope<Window> Window::CreateWindow(const WindowProps& props)
    {
        return CreateScope<CustomWindow>(props);
    }
}