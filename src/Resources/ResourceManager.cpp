#include "ResourceManager.h"
#include "../Core/Application.h"
#include <iostream>

ResourceManager& ResourceManager::Instance() {
    static ResourceManager instance;
    return instance;
}

std::shared_ptr<Shader> ResourceManager::LoadShader(const std::string& name,
                                                     const char* vertexPath,
                                                     const char* fragmentPath) {
    // 检查是否已经加载
    auto it = m_shaders.find(name);
    if (it != m_shaders.end()) {
        return it->second;
    }
    
    // 创建新shader
    auto shader = std::make_shared<Shader>(vertexPath, fragmentPath);
    m_shaders[name] = shader;
    return shader;
}

std::shared_ptr<Shader> ResourceManager::GetShader(const std::string& name) {
    auto it = m_shaders.find(name);
    if (it != m_shaders.end()) {
        return it->second;
    }
    std::cerr << "Warning: Shader '" << name << "' not found!" << std::endl;
    return nullptr;
}

std::shared_ptr<Texture> ResourceManager::LoadTexture(const std::string& name,
                                                       const char* filePath) {
    // 检查是否已经加载
    auto it = m_textures.find(name);
    if (it != m_textures.end()) {
        return it->second;
    }
    
    // 创建新texture
    auto texture = std::make_shared<Texture>(filePath);
    m_textures[name] = texture;
    return texture;
}

std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string& name) {
    auto it = m_textures.find(name);
    if (it != m_textures.end()) {
        return it->second;
    }
    std::cerr << "Warning: Texture '" << name << "' not found!" << std::endl;
    return nullptr;
}

std::shared_ptr<Geometry> ResourceManager::CreateGeometry(const std::string& name) {
    // 检查是否已经存在
    auto it = m_geometries.find(name);
    if (it != m_geometries.end()) {
        return it->second;
    }
    
    // 创建新geometry
    auto geometry = std::make_shared<Geometry>();
    m_geometries[name] = geometry;
    return geometry;
}

std::shared_ptr<Geometry> ResourceManager::GetGeometry(const std::string& name) {
    auto it = m_geometries.find(name);
    if (it != m_geometries.end()) {
        return it->second;
    }
    std::cerr << "Warning: Geometry '" << name << "' not found!" << std::endl;
    return nullptr;
}

std::shared_ptr<FrameBuffer> ResourceManager::CreateFrameBuffer(const std::string& name,
                                                                 int width, int height) {
    // 检查是否已经存在
    auto it = m_frameBuffers.find(name);
    if (it != m_frameBuffers.end()) {
        return it->second;
    }
    
    // 创建新framebuffer
    auto framebuffer = std::make_shared<FrameBuffer>(name, width, height);
    m_frameBuffers[name] = framebuffer;
    return framebuffer;
}

std::shared_ptr<FrameBuffer> ResourceManager::GetFrameBuffer(const std::string& name) {
    auto it = m_frameBuffers.find(name);
    if (it != m_frameBuffers.end()) {
        return it->second;
    }
    std::cerr << "Warning: FrameBuffer '" << name << "' not found!" << std::endl;
    return nullptr;
}

void ResourceManager::UnloadShader(const std::string& name) {
    m_shaders.erase(name);
}

void ResourceManager::UnloadTexture(const std::string& name) {
    m_textures.erase(name);
}

void ResourceManager::UnloadGeometry(const std::string& name) {
    m_geometries.erase(name);
}

void ResourceManager::UnloadFrameBuffer(const std::string& name) {
    m_frameBuffers.erase(name);
}

void ResourceManager::Clear() {
    m_shaders.clear();
    m_textures.clear();
    m_geometries.clear();
    m_frameBuffers.clear();
}
