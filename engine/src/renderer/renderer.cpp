#include <kent/renderer/renderer.h>

namespace Kent {

    Scope<Renderer> Renderer::CreateRenderer() 
    {
        return CreateScope<Renderer>();
    }
}