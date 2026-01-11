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

enum class Rotation {
    X,
    Y,
    Z
};

class Renderable {
private:

    // Constructions of Renderables
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GLenum draw_state = GL_STATIC_DRAW;

    VAO vao;
    VBO vbo;
    EBO ebo;

    Shader shader;
    Texture2D texture2D;

    // Modifiable properties of Renderables
    glm::vec3 position{0.f};
    glm::vec3 rotation{0.f};
    glm::vec3 scale{1.f};

    glm::mat4 model = glm::mat4(1.f);

    // Flags
    bool using_texture = false; // by default, you aren't using a texture, unless you specify
    bool dirty_drawstate = false; // Enabled when draw_state changes
    bool dirty_indices = false; // Enabled when the indices are changed via SetIndices()

    // Model calculations
    int modelLoc = -1; // No model location (yet)
    void UniformCalculations();

    void Create(); // Simpler create function so I don't have to retype everything. The constructors have similar (almost 1:1)
    // ways of creating a renderable
public:
    Renderable(std::vector<Vertex>& vert, std::vector<GLuint>& ind, Shader& _shader, Texture2D _texture);
    Renderable(std::vector<Vertex>& vert, std::vector<GLuint>& ind, Shader& _shader);
    ~Renderable();

    // Incremental functions, basically increments current state by the given one
    virtual void Move(const glm::vec3& increment);
    virtual void Scale(const glm::vec3& increment);
    virtual void Rotate(const glm::vec3& degrees);

    // Setter-functions. Sets whatever value you need it to be
    virtual void SetPosition(const glm::vec3& new_pos);
    virtual void SetScale(const glm::vec3& new_scale);
    virtual void SetRotation(const glm::vec3& new_rotation);
    virtual void SetDrawState(GLenum state);
    virtual void SetIndices(const std::vector<GLuint>& ind);

    // TODO: Recreate shaders and texture2D fuctions with flexibel system to access them
    virtual void RecreateShaders(const char* FragmentSourceFilepath, const char* VertexSourceFilepath);
    virtual void RecreateTexture2D(const char* TextureFilepath, TextureSettings settings = TextureSettings());

    //TODO: Enable/Disable Textures (or removing)
    
    
    // Getter-functions
    virtual Shader GetShader() const;
    virtual Texture2D GetTexture2D() const;
    

    // Graphics
    virtual void Draw();
    virtual void Update();
};