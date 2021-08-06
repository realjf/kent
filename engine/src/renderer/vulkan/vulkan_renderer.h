#pragma once 

#include <kent/renderer/renderer.h>

namespace Kent {

    class VulkanRenderer : public Renderer {
    public:
        virtual void Init(RendererSettings settings) override;
        virtual void Shutdown() override;
        virtual void RenderFrame() override;
    };

}

