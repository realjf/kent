#include <kent/engine.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "kent/service_locator.h"
#include "platform/glfw_window.h"

namespace Kent
{

    void Engine::Init()
    {
        std::cout << "Hello, I'm kent engine" << std::endl;

        ServiceLocator::Provide(new CustomWindow);
    }

}
