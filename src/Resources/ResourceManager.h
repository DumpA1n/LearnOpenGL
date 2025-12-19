#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "Shader.h"
#include "Texture.h"
#include "Geometry.h"
#include "FrameBuffer.h"

// 资源管理器 - 单例模式
class ResourceManager {
public:
    // 获取单例实例
    static ResourceManager& Instance();
    
    // 禁止拷贝和赋值
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    
    // Shader资源管理
    std::shared_ptr<Shader> LoadShader(const std::string& name, 
                                       const char* vertexPath, 
                                       const char* fragmentPath);
    std::shared_ptr<Shader> GetShader(const std::string& name);
    
    // Texture资源管理
    std::shared_ptr<Texture> LoadTexture(const std::string& name, 
                                         const char* filePath);
    std::shared_ptr<Texture> GetTexture(const std::string& name);
    
    // Geometry资源管理
    std::shared_ptr<Geometry> CreateGeometry(const std::string& name);
    std::shared_ptr<Geometry> GetGeometry(const std::string& name);
    
    // FrameBuffer资源管理
    std::shared_ptr<FrameBuffer> CreateFrameBuffer(const std::string& name, 
                                                   int width, int height);
    std::shared_ptr<FrameBuffer> GetFrameBuffer(const std::string& name);
    
    // 清理指定资源
    void UnloadShader(const std::string& name);
    void UnloadTexture(const std::string& name);
    void UnloadGeometry(const std::string& name);
    void UnloadFrameBuffer(const std::string& name);
    
    // 清理所有资源
    void Clear();
    
    // 获取资源统计信息
    size_t GetShaderCount() const { return m_shaders.size(); }
    size_t GetTextureCount() const { return m_textures.size(); }
    size_t GetGeometryCount() const { return m_geometries.size(); }
    size_t GetFrameBufferCount() const { return m_frameBuffers.size(); }

private:
    ResourceManager() = default;
    ~ResourceManager() = default;
    
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
    std::unordered_map<std::string, std::shared_ptr<Geometry>> m_geometries;
    std::unordered_map<std::string, std::shared_ptr<FrameBuffer>> m_frameBuffers;
};
