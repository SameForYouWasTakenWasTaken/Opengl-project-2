#pragma once
#include <vector>
#include <concepts>

#include <glad/gl.h>

#include "LowLevelShit/Vertex.hpp"

class VBO final {
    GLuint id;
public:
    VBO();
    ~VBO();

    void SetData(std::vector<Vertex>& vertices, GLenum draw_type = GL_STATIC_DRAW);

    void Bind();
    void Unbind();

};