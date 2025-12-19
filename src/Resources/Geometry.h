#pragma once

#include "glad/glad.h"
#include <vector>
#include <cstddef>

// 顶点属性描述
struct VertexAttribute {
    GLuint index;           // 属性位置
    GLint size;             // 组件数量 (1, 2, 3, 4)
    GLenum type;            // 数据类型 (GL_FLOAT, GL_INT, etc)
    GLboolean normalized;   // 是否归一化
    GLsizei stride;         // 步长
    const void* offset;     // 偏移量
};

class Geometry {
public:
    Geometry();
    ~Geometry();

    // 禁止拷贝
    Geometry(const Geometry&) = delete;
    Geometry& operator=(const Geometry&) = delete;

    // 设置顶点数据
    void setVertexData(const float* vertices, size_t size, GLenum usage = GL_STATIC_DRAW);
    void updateVertexData(const float* vertices, size_t size, size_t offset = 0);
    
    // 设置索引数据
    void setIndexData(const unsigned int* indices, size_t count, GLenum usage = GL_STATIC_DRAW);
    
    // 设置顶点属性
    void setVertexAttribute(GLuint index, GLint size, GLenum type, 
                           GLboolean normalized, GLsizei stride, const void* offset);
    
    // 便捷方法：设置标准顶点属性布局 (位置 + 纹理坐标)
    void setStandardLayout();
    
    // 绑定/解绑
    void bind() const;
    void unbind() const;
    
    // 绘制
    void draw(GLenum mode = GL_TRIANGLES) const;
    void drawElements(GLenum mode, GLsizei count, GLenum type = GL_UNSIGNED_INT) const;
    void drawArrays(GLenum mode, GLint first, GLsizei count) const;
    
    // 获取OpenGL对象
    GLuint getVAO() const { return m_VAO; }
    GLuint getVBO() const { return m_VBO; }
    GLuint getEBO() const { return m_EBO; }
    
    size_t getIndexCount() const { return m_indexCount; }

private:
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;
    size_t m_indexCount;
    bool m_hasIndices;
};
