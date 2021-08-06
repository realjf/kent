#include <kent/renderer/renderer.h>
#include "vulkan/vulkan_renderer.h"
namespace Kent {

    Scope<Renderer> Renderer::CreateRenderer() 
    {
        return CreateScope<VulkanRenderer>();
    }
}