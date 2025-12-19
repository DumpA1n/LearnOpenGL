#include "MTFilter.h"
#include "Core/Application.h"
#include "Resources/ResourceManager.h"

std::shared_ptr<FrameBuffer> MTFilter::newFrameBuffer(std::string name) {
    Application* app = Application::GetInstance();
    int width = app ? app->GetWidth() : 1200;
    int height = app ? app->GetHeight() : 800;
    return ResourceManager::Instance().CreateFrameBuffer(name, width, height);
}
