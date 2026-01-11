#include "Drawable.hpp"

Drawable::Drawable(std::vector<Vertex>& vert, std::vector<GLuint>& ind, Shader& _shader, Texture2D _texture) :
vertices(vert),
indices(ind),
shader(_shader),
texture2D(_texture)
{
    // ? These settings won't change no matter what for Renderables. This is standard config for Vertex's
    Create();
    using_texture = true; // This is the constructor for textures
}

Drawable::Drawable(std::vector<Vertex>& vert, std::vector<GLuint>& ind, Shader& _shader)
:
vertices(vert),
indices(ind),
shader(_shader)
{
    Create(); // Won't turn on using_texture by default, so this could be the standard constructor if you will
}

Drawable::~Drawable()
{

}

// ! NOT PROPERLY TESTED
void Drawable::SetDrawState(GLenum state)
{
    drawState = state;
    dirty_buffers = true;
}

// ! NOT PROPERLY TESTED
void Drawable::SetIndices(const std::vector<GLuint>& ind)
{
    indices = ind;
    dirty_indices = true;
}


Shader Drawable::GetShader() const
{
    return shader;
}

Texture2D Drawable::GetTexture2D() const
{
    return texture2D;
}

void Drawable::RecreateShaders(const char* FragmentSourceFilepath, const char* VertexSourceFilepath)
{
    shader.ResetShaders(FragmentSourceFilepath, VertexSourceFilepath);
    shader.UseProgram();
    shader.UnuseProgram(); // Stop using just incase any bugs appear.
}

void Drawable::RecreateTexture2D(const char* TextureFilepath, TextureSettings settings)
{
    texture2D.SetSettings(settings);
    texture2D.Recreate(TextureFilepath);
    if (glIsTexture(texture2D.GetTexture() == GL_FALSE))
    {
        spdlog::error("Something went wrong when recreating the texture! Texture does not exist");
    }
    using_texture = true;
}

const std::vector<GLuint>& Drawable::GetIndices() const
{
    return indices;
}
const std::vector<Vertex>& Drawable::GetVertices() const
{
    return vertices;
}

void Drawable::Create()
{
    VertexAttributePointer pos;
    pos.index = 0;
    pos.size = 3;
    pos.stride = Vertex::stride();
    pos.offset = (void*)offsetof(Vertex, position);

    VertexAttributePointer col;
    col.index = 1;
    col.size = 4;
    col.stride = Vertex::stride();
    col.offset = (void*)offsetof(Vertex, color);
    
    VertexAttributePointer tex;
    tex.index = 2;
    tex.size = 2;
    tex.stride = Vertex::stride();
    tex.offset = (void*)offsetof(Vertex, texture);

    // GPU magic
    vao.Bind();

    vbo.Bind();
    vbo.SetData(vertices, drawState);

    ebo.Bind();
    ebo.SetData(indices, drawState);

    vao.LinkVertexAttributePointer(pos);
    vao.LinkVertexAttributePointer(col);
    vao.LinkVertexAttributePointer(tex);
    
    vbo.Unbind();
}