#include <kent/core/entry_point.h>
#include <kent/engine.h>

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
