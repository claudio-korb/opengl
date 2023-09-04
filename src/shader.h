#pragma once
#include <cstring>
#include <fstream>
#include <sstream>
#include <unordered_map> 
#include "logs.h"
#include "vendor/glm/glm.hpp"

typedef struct 
{
    std::string VertexSource;
    std::string FragmentSource;
}ShaderProgramSource;

class Shader
{
private:
    std::string m_FilePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
public:
    Shader(const std::string& filepath);
    ~Shader();
    void Bind() const;
    void Unbind() const;

    //set uniforms
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
    void SetUniform1i(const std::string& name, int value);

private:
    unsigned int GetUniformLocation(const std::string& name);
    ShaderProgramSource ParseShader(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string fragmentShader);
};