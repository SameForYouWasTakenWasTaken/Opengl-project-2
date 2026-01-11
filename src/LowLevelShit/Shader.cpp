#include <spdlog/spdlog.h>

#include "LowLevelShit/Shader.hpp"

/* 
It returns a pair, which consists of two string_views, the first being the Fragment Source .glsl code, the other being the Vertex Source .glsl code.
Should an error occur, it will return an int with an error code -1.

@param FragmentSourceFilePath       The file, in which the fragment source code resides in
@param VertexSourceFilePath         The file, in which the vertex source code resides in
*/
std::expected<std::pair<std::string, std::string>, int>
ParseShaderFiles(const char* FragmentSourceFilePath, const char* VertexSourceFilePath)
{
    std::ifstream fragment(FragmentSourceFilePath); // fragment contents
    std::ifstream vertex(VertexSourceFilePath); // vertex contents

    if (!fragment.is_open() || !vertex.is_open()) {
        return std::unexpected(-1);
    }

    std::stringstream frag_buffer;
    std::stringstream vert_buffer;


    frag_buffer << fragment.rdbuf();
    vert_buffer << vertex.rdbuf();

    return std::make_pair(frag_buffer.str(), vert_buffer.str());
}

Shader::Shader(const char* FragmentSourceFilePath, const char* VertexSourceFilePath)
{
    auto result = ParseShaderFiles(FragmentSourceFilePath, VertexSourceFilePath);
    if(!result)
    {
        spdlog::error("Couldn't retrieve fragment/vertex source files!");
        return;
    }

    auto ShaderSources = result.value();

    SetShaderSources(ShaderSources.first.c_str(), ShaderSources.second.c_str());
    ResetShaders(); // Forward to resetshaders, since it already handles all of the shader creation
}

Shader::~Shader()
{
    if (glIsProgram(ShaderProgram) == GL_TRUE)
        glDeleteProgram(ShaderProgram);

    // Usually not the case, since they *should* get deleted anyway, but honestly it doesnt hurt to put this here
    if (glIsShader(VertexShader) == GL_TRUE)
        glDeleteShader(VertexShader);
    
    if (glIsShader(FragmentShader) == GL_TRUE)
        glDeleteShader(FragmentShader);
}

void Shader::ResetShaders(const char* FragmentSourceFilepath, const char* VertexSourceFilepath)
{
    auto result = ParseShaderFiles(FragmentSourceFilepath, VertexSourceFilepath);
    if(!result)
    {
        spdlog::error("Couldn't retrieve fragment/vertex source files!");
        return;
    }

    auto ShaderSources = result.value();
    SetShaderSources(ShaderSources.first.c_str(), ShaderSources.second.c_str());
    ResetShaders(); // Forward to resetshaders, since it already handles all of the shader creation
}

void Shader::ResetShaders()
{
    if (GLSL_FragmentShaderSource == nullptr || GLSL_VertexShaderSource == nullptr)
    {
        spdlog::error("Fragment or vertex shader source is not properly defined!");
        return;
    }

    if (glIsProgram(ShaderProgram) == GL_TRUE)
        glDeleteProgram(ShaderProgram);

    // Vertex
    VertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(VertexShader, 1, &GLSL_VertexShaderSource, NULL);
    glCompileShader(VertexShader);

    // Fragment
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &GLSL_FragmentShaderSource, NULL);
    glCompileShader(FragmentShader);


    // Program
    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);

    glLinkProgram(ShaderProgram);

    // Debug    
    int success;
    char infoLog[512];
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    
    if(!success)
    {
        glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
        glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
        glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
        spdlog::error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n {}", infoLog);
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    SetInt("ShaderTexture", 0);
}

void Shader::UseProgram()
{
    if (glIsProgram(ShaderProgram) == GL_FALSE)
    {
         spdlog::error("Shader program not available?");
        return;
    }
    glUseProgram(ShaderProgram);
}

void Shader::UnuseProgram()
{
    if (glIsProgram(ShaderProgram) == GL_FALSE)
    {
        spdlog::error("Shader program not available?");
        return;
    }

    glUseProgram(0); // ! If you put this in the if statement above regardless, it might unuse a different shader program which might be currently in use that isnt this one !
}

void Shader::SetShaderSources(const char* FragmentShaderSource, const char* VertexShaderSource)
{
    GLSL_VertexShaderSource = VertexShaderSource;
    GLSL_FragmentShaderSource = FragmentShaderSource;
}

void Shader::SetInt(const char* name, int n)
{
    int loc = GetUniformLocation(name);
    if (loc != -1)
        glUniform1i(loc, n);
}

void Shader::SetFloat(const char* name, float n)
{
    int loc = GetUniformLocation(name);
    if (loc != -1)
        glUniform1f(loc, n);
}    
    
// Typically reserved for one time uses
void Shader::SetMatrix4(const char* name, int amount, const GLfloat* value)
{
    int loc = GetUniformLocation(name);
    if (loc != -1)
        glUniformMatrix4fv(loc, amount, GL_FALSE, value);
}

// Typically reserved if you already have a location and simply don't want to rewrite the whole function.
// Either way, looks prettier. Totally optional, of course.
void Shader::SetMatrix4(int loc, int amount, const GLfloat* value)
{
    glUniformMatrix4fv(loc, amount, GL_FALSE, value);
}


int Shader::GetUniformLocation(const char* name)
{
    if (glIsProgram(ShaderProgram))
        return glGetUniformLocation(ShaderProgram, name);

    return -1;
}