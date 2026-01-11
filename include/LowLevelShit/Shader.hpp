#pragma once

#include <glad/gl.h>
#include <utility>
#include <fstream>
#include <expected>
#include <sstream>
#include <string>

extern std::expected<std::pair<std::string, std::string>, int> 
ParseShaderFiles(const char* FragmentSourceFile, const char* VertexSourceFile);

class Shader final {
    GLuint ShaderProgram, FragmentShader, VertexShader;
    
    // GLSL source code
    const char* GLSL_VertexShaderSource;
    const char* GLSL_FragmentShaderSource;
public:
    /*
    @param FragmentSourceFile       The file, in which the fragment source code resides in
    @param VertexSourceFile         The file, in which the vertex source code resides in
    */
    explicit Shader(const char* FragmentSourceFile, const char* VertexSourceFile);
    ~Shader();

    void ResetShaders();
    void ResetShaders(const char* FragmentSourceFilepath, const char* VertexSourceFilepath);
    void SetShaderSources(const char* VertexShaderSource, const char* FragmentShaderSource); // This does NOT take in file paths!
    void UseProgram();
    void UnuseProgram();
    // ? Uniform functions ?
    void SetInt(const char* name, int n);
    void SetFloat(const char* name, float n);
    
    void SetMatrix4(const char* name, int amount, const GLfloat* value);
    void SetMatrix4(int loc, int amount, const GLfloat* value);
    
    int GetUniformLocation(const char* name); // Easier to do than just glGetUniform(shader.GetProgram(), "x")

    // Getters
    GLuint GetProgram() const {return ShaderProgram;} 
};