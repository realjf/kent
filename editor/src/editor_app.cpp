#include <kent_engine/platform/entry_point.h>
#include <kent_engine/platform/application.h>

class Editor : public Kent::Application {
public:
    Editor() {
        std::cout << "Editor" << std::endl;
    }
    virtual ~Editor() = default;
};

Kent::Application* Kent::CreateApplication() {
    return new Editor();
}
