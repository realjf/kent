#pragma once

#include <iostream>
#include <kent_engine/engine.h>
#include <kent_engine/service_locator.h>
#include <kent_engine/platform/application.h>


int main(int argc, char** argv) {
    auto* app = Kent::CreateApplication();


    std::cout << "Hello World!" << std::endl;
    Kent::Engine::Init();

    Kent::ServiceLocator::GetWindow()->CreateWindow();

    Kent::ServiceLocator::GetWindow()->Run();
    
    
    return 0;
}
