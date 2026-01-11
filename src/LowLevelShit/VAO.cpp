#include "LowLevelShit/VAO.hpp"

VAO::VAO()
{
    glGenVertexArrays(1, &id);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &id);
}

void VAO::Bind()
{
    glBindVertexArray(id);
}

void VAO::Unbind()
{
    glBindVertexArray(0);
}

void VAO::LinkVertexAttributePointer(VertexAttributePointer attribute)
{
    glVertexAttribPointer(
        attribute.index, 
        attribute.size, 
        attribute.type, 
        attribute.normalized, 
        attribute.stride,
        attribute.offset);
    glEnableVertexAttribArray(attribute.index);
}