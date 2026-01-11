#include "Renderable.hpp"

Renderable::Renderable(std::vector<Vertex>& vert, std::vector<GLuint>& ind, Shader& _shader, Texture2D _texture) :
vertices(vert),
indices(ind),
shader(_shader),
texture2D(_texture)
{
    // ? These settings won't change no matter what for Renderables. This is standard config for Vertex's
    Create();
    using_texture = true; // This is the constructor for textures
}

Renderable::Renderable(std::vector<Vertex>& vert, std::vector<GLuint>& ind, Shader& _shader)
:
vertices(vert),
indices(ind),
shader(_shader)
{
    Create(); // Won't turn on using_texture by default, so this could be the standard constructor if you will
}

Renderable::~Renderable()
{

}

void Renderable::Move(const glm::vec3& increment)
{
    position += increment;
}

void Renderable::Rotate(const glm::vec3& increment)
{
    rotation += increment;
    rotation = glm::mod(rotation, glm::vec3(360.f));
}

void Renderable::Scale(const glm::vec3& increment)
{
    scale += increment;
}

void Renderable::SetPosition(const glm::vec3& pos)
{
    position = pos;
}

void Renderable::SetRotation(const glm::vec3& rot)
{
    rotation = rot;
}
void Renderable::SetScale(const glm::vec3& _scale)
{
    scale = _scale;
}

// ! NOT PROPERLY TESTED
void Renderable::SetDrawState(GLenum state)
{
    draw_state = state;
    dirty_drawstate = true;
}

// ! NOT PROPERLY TESTED
void Renderable::SetIndices(const std::vector<GLuint>& ind)
{
    indices = ind;
    dirty_indices = true;
}

void Renderable::Draw()
{
    shader.UseProgram();
    if (using_texture)
        texture2D.Use();
    UniformCalculations();
    vao.Bind();

    if (dirty_indices)
    {
        ebo.Bind();
        ebo.SetData(indices, draw_state);
        ebo.Unbind();
        dirty_indices = false;
    }

    if (dirty_drawstate)
    {
        vbo.Bind();
        vbo.SetData(vertices, draw_state);
        vbo.Unbind();
        dirty_drawstate = false;
    }

}

void Renderable::Update()
{

}


Shader Renderable::GetShader() const
{
    return shader;
}

Texture2D Renderable::GetTexture2D() const
{
    return texture2D;
}

void Renderable::RecreateShaders(const char* FragmentSourceFilepath, const char* VertexSourceFilepath)
{
    shader.ResetShaders(FragmentSourceFilepath, VertexSourceFilepath);
    shader.UseProgram();
    UniformCalculations();
    shader.UnuseProgram(); // Stop using just incase any bugs appear.
}

void Renderable::RecreateTexture2D(const char* TextureFilepath, TextureSettings settings)
{
    texture2D.SetSettings(settings);
    texture2D.Recreate(TextureFilepath);
    if (glIsTexture(texture2D.GetTexture() == GL_FALSE))
    {
        spdlog::error("Something went wrong when recreating the texture! Texture does not exist");
    }
    using_texture = true;
}

// <---------------- Private member functions ----------------> //
void Renderable::UniformCalculations()
{
    model = glm::mat4(1.f);

    model = glm::translate(model, position);
    
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
    
    model = glm::scale(model, scale);

    if (modelLoc == -1) {
        spdlog::error("No model location!");
        return;
    }

    shader.SetMatrix4("model", 1, glm::value_ptr(model));
}

void Renderable::Create()
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
    vbo.SetData(vertices, draw_state);

    ebo.Bind();
    ebo.SetData(indices, draw_state);

    vao.LinkVertexAttributePointer(pos);
    vao.LinkVertexAttributePointer(col);
    vao.LinkVertexAttributePointer(tex);

    vbo.Unbind();
    // TODO: Cache modelloc and re-cache later when changing shaders
    modelLoc = shader.GetUniformLocation("model");
    spdlog::info("Model location: {}", modelLoc);
}