#include "../include/shader.h"

Shader::Shader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    if(!stream)
    {
        std::cout << "Unable to open file at: " << filepath << std::endl;
    }

    enum class ShaderType
    {
        SHADER_NONE=-1, SHADER_VERTEX=0, SHADER_FRAGMENT=1
    };

    ShaderType type = ShaderType::SHADER_NONE;

    std::string line;
    std::stringstream ss[2];

    while(getline(stream, line))
    {
        if(line.find("#shader") != std::string::npos)
        {
            if(line.find("vertex") != std::string::npos)
            {
                type = ShaderType::SHADER_VERTEX;
            }
            else if(line.find("fragment") != std::string::npos)
            {
                type = ShaderType::SHADER_FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    this->sources.FragmentSource = ss[(int)ShaderType::SHADER_FRAGMENT].str();
    this->sources.VertexSource = ss[(int)ShaderType::SHADER_VERTEX].str();
    this->filepath = filepath;
};

ShaderProgramSource Shader::GetShaderSources()
{
    return this->sources;
};

unsigned int Shader::GetProgram()
{
    return this->program;
};

void Shader::CompileShaders()
{
    unsigned int vertex_id = glCreateShader(GL_VERTEX_SHADER);
    const char* src = this->sources.VertexSource.c_str();
    glShaderSource(vertex_id, 1, &src, nullptr);
    glCompileShader(vertex_id);

    int compile_result;
    glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &compile_result);
    if(compile_result == GL_FALSE)
    {
        int length;
        glGetShaderiv(vertex_id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(vertex_id, length, &length, message);
        std::cout << "Failed to compile Vertex shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(vertex_id);
        return;
    }

    unsigned int fragment_id = glCreateShader(GL_FRAGMENT_SHADER);

    src = this->sources.FragmentSource.c_str();
    glShaderSource(fragment_id, 1, &src, nullptr);
    glCompileShader(fragment_id);

    glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &compile_result);
    if(compile_result == GL_FALSE)
    {
        int length;
        glGetShaderiv(fragment_id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(fragment_id, length, &length, message);
        std::cout << "Failed to compile Fragment shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(fragment_id);
        return;
    }

    unsigned int program = glCreateProgram();

    glAttachShader(program, vertex_id);
    glAttachShader(program, fragment_id);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);

    this->program = program;

};

Shader::~Shader()
{
    glDeleteProgram(this->program);
}