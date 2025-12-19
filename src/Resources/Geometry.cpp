#include "Geometry.h"

Geometry::Geometry()
    : m_VAO(0)
    , m_VBO(0)
    , m_EBO(0)
    , m_indexCount(0)
    , m_hasIndices(false)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
}

Geometry::~Geometry() {
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_EBO) glDeleteBuffers(1, &m_EBO);
}

void Geometry::setVertexData(const float* vertices, size_t size, GLenum usage) {
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
}

void Geometry::updateVertexData(const float* vertices, size_t size, size_t offset) {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
}

void Geometry::setIndexData(const unsigned int* indices, size_t count, GLenum usage) {
    m_hasIndices = true;
    m_indexCount = count;
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, usage);
}

void Geometry::setVertexAttribute(GLuint index, GLint size, GLenum type,
                                  GLboolean normalized, GLsizei stride, const void* offset) {
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(index, size, type, normalized, stride, offset);
    glEnableVertexAttribArray(index);
}

void Geometry::setStandardLayout() {
    // 位置属性 (location = 0): 3个float
    setVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // 纹理坐标属性 (location = 1): 2个float
    setVertexAttribute(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Geometry::bind() const {
    glBindVertexArray(m_VAO);
}

void Geometry::unbind() const {
    glBindVertexArray(0);
}

void Geometry::draw(GLenum mode) const {
    bind();
    if (m_hasIndices) {
        glDrawElements(mode, m_indexCount, GL_UNSIGNED_INT, 0);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Geometry::drawElements(GLenum mode, GLsizei count, GLenum type) const {
    bind();
    glDrawElements(mode, count, type, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Geometry::drawArrays(GLenum mode, GLint first, GLsizei count) const {
    bind();
    glDrawArrays(mode, first, count);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
