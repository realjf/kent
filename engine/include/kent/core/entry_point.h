#pragma once

#include <iostream>
#include <kent/engine.h>
#include <kent/core/application.h>


int main(int argc, char** argv) {
    auto* app = Kent::CreateApplication();

    app->Run();
    
    app->Close();

    delete app;
    return 0;
}
