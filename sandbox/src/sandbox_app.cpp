#include <kent/core/entry_point.h>
#include <kent/engine.h>

class Sandbox : public Kent::Application {
public:
    Sandbox() : Kent::Application("Sandbox") {
        std::cout << "Sandbox" << std::endl;
    }
    virtual ~Sandbox() = default;
};

Kent::Application* Kent::CreateApplication() {
    return new Sandbox();
}


