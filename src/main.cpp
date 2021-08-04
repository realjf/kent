#include <kent_engine/platform/entry_point.h>
#include <kent_engine/platform/application.h>

class Sandbox : public Kent::Application {
public:
    Sandbox() {
        std::cout << "Sandbox" << std::endl;
    }
    virtual ~Sandbox() = default;
};

Kent::Application* Kent::CreateApplication() {
    return new Sandbox();
}


