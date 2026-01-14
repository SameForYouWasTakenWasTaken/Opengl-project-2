#pragma once
#include <expected>
#include <spdlog/spdlog.h>
#include <glad/gl.h>

#include "LowLevelShit/Texture2D.hpp"
#include "LowLevelShit/Vertex.hpp"
#include "LowLevelShit/Shader.hpp"
#include "LowLevelShit/VAO.hpp"
#include "LowLevelShit/VBO.hpp"
#include "LowLevelShit/EBO.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class DrawMode {
    Elements,
    Arrays
};

class Drawable {
private:

    // Constructions of Renderables
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    void Create(); // Simpler create function so I don't have to retype everything. The constructors have similar (almost 1:1)
    // ways of creating a renderable
public:
    Drawable(std::vector<Vertex>& vert, std::vector<GLuint>& ind, Shader& _shader, Texture2D _texture);
    Drawable(std::vector<Vertex>& vert, std::vector<GLuint>& ind, Shader& _shader);
    ~Drawable();

    virtual void SetDrawState(GLenum state);
    virtual void SetIndices(const std::vector<GLuint>& ind);

    // TODO: Recreate shaders and texture2D fuctions with flexibel system to access them
    virtual void RecreateShaders(const char* FragmentSourceFilepath, const char* VertexSourceFilepath);
    virtual void RecreateTexture2D(const char* TextureFilepath, TextureSettings settings = TextureSettings());

    //TODO: Enable/Disable Textures (or removing)
    
    // Getter-functions
    virtual Shader GetShader() const;
    virtual Texture2D GetTexture2D() const;
    virtual const std::vector<GLuint>& GetIndices() const;
    virtual const std::vector<Vertex>& GetVertices() const;

    // Graphics
    VAO vao;
    VBO vbo;
    EBO ebo;
    
    Shader shader;
    Texture2D texture2D;
    
    // Draw stuff
    GLenum drawState = GL_STATIC_DRAW;
    GLenum primitive = GL_TRIANGLES;

    DrawMode drawMode = DrawMode::Elements;
    // Flags
    bool using_texture = false; // by default, you aren't using a texture, unless you specify
    bool dirty_indices = false; // Enabled when the indices are changed via SetIndices()
    bool dirty_buffers = false;
};