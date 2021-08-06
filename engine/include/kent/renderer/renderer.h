#pragma once

#include <kent/core/reference.h>

namespace Kent {

    struct RendererSettings {

    };

    class Renderer {
    public:
        virtual void Init(RendererSettings settings) = 0;
        virtual void Shutdown() = 0;
        virtual void RenderFrame() = 0;

        static Scope<Renderer> CreateRenderer();
    };

}

