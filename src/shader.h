#pragma once
#include <cstring>
#include <fstream>
#include <sstream>
#include "renderer.h"

typedef struct 
{
    std::string VertexSource;
    std::string FragmentSource;
}ShaderProgramSource;

class Shader
{
    private:
        std::string filepath;
        ShaderProgramSource sources;
        unsigned int program;
    public:
        Shader(const std::string& filepath);
        ~Shader();
        void CompileShaders();
        ShaderProgramSource GetShaderSources();
        unsigned int GetProgram();
};
