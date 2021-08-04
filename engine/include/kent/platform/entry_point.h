#pragma once

#include <iostream>
#include <kent/engine.h>
#include <kent/service_locator.h>
#include <kent/platform/application.h>


int main(int argc, char** argv) {
    auto* app = Kent::CreateApplication();


    std::cout << "Hello World!" << std::endl;
    Kent::Engine::Init();

    Kent::ServiceLocator::GetWindow()->CreateWindow();

    Kent::ServiceLocator::GetWindow()->Run();
    
    
    return 0;
}
