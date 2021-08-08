#pragma once

#include <kent/core/reference.h>
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <string>
#include <unordered_map>
#include <any>
namespace Kent {

    struct RendererSettings {
        std::string ApplicationName;
    };

    enum class SurfaceArgs {
        INSTANCE,
        ALLOCATORS,
        OUT_SURFACE,
    };

    class Renderer {
    public:
        virtual void Init(RendererSettings settings, GLFWwindow* window, int width, int height) = 0;
        virtual void Shutdown() = 0;
        virtual void RenderFrame() = 0;

        static Scope<Renderer> CreateRenderer();

        virtual void DrawSurface(std::unordered_map<SurfaceArgs, std::any> args, GLFWwindow* window) = 0;

    private:

    };

}

