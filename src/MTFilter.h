#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "Resources/Shader.h"
#include "Resources/FrameBuffer.h"
#include "Resources/Texture.h"
#include "Resources/Geometry.h"
#include "Resources/ResourceManager.h"
#include "MTFilterManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// 前向声明
class Application;

class MTFilter {
public:
    MTFilter() = default;
    virtual ~MTFilter() = default;

    // 使用ResourceManager加载资源
    std::shared_ptr<Shader> newShader(std::string name, const char* vert, const char* frag) {
        return ResourceManager::Instance().LoadShader(name, vert, frag);
    }
    
    std::shared_ptr<Shader> getShader(std::string name) {
        return ResourceManager::Instance().GetShader(name);
    }

    std::shared_ptr<FrameBuffer> newFrameBuffer(std::string name);
    
    std::shared_ptr<FrameBuffer> getFrameBuffer(std::string name) {
        return ResourceManager::Instance().GetFrameBuffer(name);
    }

    std::shared_ptr<Texture> newTexture(std::string name, const char* filename) {
        return ResourceManager::Instance().LoadTexture(name, filename);
    }
    
    std::shared_ptr<Texture> getTexture(std::string name) {
        return ResourceManager::Instance().GetTexture(name);
    }
    
    std::shared_ptr<Geometry> newGeometry(std::string name) {
        return ResourceManager::Instance().CreateGeometry(name);
    }
    
    std::shared_ptr<Geometry> getGeometry(std::string name) {
        return ResourceManager::Instance().GetGeometry(name);
    }
};
