#include "Core/Application.h"
#include "demo/TestFilter1_ECS.h"
#include <iostream>

int main()
{
    Application app(1200, 800, "Renderer");
    
    if (!app.Init()) {
        std::cerr << "Failed to initialize application" << std::endl;
        return -1;
    }
    
    // 可选：使用Scene系统
    auto scene = TestFilter1_ECS::CreateScene();
    app.SetActiveScene(scene);
    
    app.Run();
    
    return 0;
}