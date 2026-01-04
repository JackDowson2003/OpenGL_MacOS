///
/// Created by chenlong on 2026/1/3.
///


#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Shader.h"
#include "Renderer.h"


Shader::Shader(const std::string &filepath)
        : m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
    GLCall(glUseProgram(0));
}

void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(Uniforms(name), v0, v1, v2, v3));
}

void Shader::setUniform1f(const std::string &name, float value)
{
    GLCall(glUniform1f(Uniforms(name), value));
}

int Shader::Uniforms(const std::string &name)
{
    //find cache from hashmap
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    ///GET DATA OF MY SHADER
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "[Warning]: uniform '" << name << "' doesn't exist here" << std::endl;

    m_UniformLocationCache[name] = location; //add cache
    return location;
}


ShaderProgramSource Shader::ParseShader(const std::string &filePath)
{
    std::ifstream ifstream(filePath);
    std::vector<int> vec;

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2]; //vertex fragment
    ShaderType type = ShaderType::NONE;
    while (getline(ifstream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        } else
        {
            ss[(int) type] << line << '\n';
        }
    }
    return {ss[0].str(), ss[1].str()};
}

/**
 * 编译着色器函数
 * @param type 着色器类型（GL_VERTEX_SHADER 或 GL_FRAGMENT_SHADER）
 * @param source 着色器源代码字符串
 * @return 返回编译成功后的着色器ID，若编译失败返回0
 */
unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
    // 创建指定类型的着色器对象
    GLCall(unsigned int id = glCreateShader(type));
    // 获取源代码的C风格字符串指针
    const char *src = source.c_str();
    // 将源代码附加到着色器对象
    GLCall(glShaderSource(id, 1, &src, nullptr));
    // 编译着色器
    GLCall(glCompileShader(id));

    // 检查编译状态
    int result;
    // 查询着色器的编译状态
    // glGetShaderiv 返回的信息将存储在 result 中，用于判断着色器是否编译成功
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        // 如果编译失败，获取错误信息长度
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        // 使用alloca在栈上分配内存存储错误信息
        char *message = (char *) alloca(sizeof(char) * length);
        // 获取并输出错误信息
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Compiled " << ((type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment") << " Shader Error"
                  << std::endl;
        std::cout << message << std::endl;
        // 删除编译失败的着色器
        GLCall(glDeleteShader(id));
        return 0; // 返回0表示编译失败
    }
    return id; // 返回编译成功的着色器ID
}

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram(););
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    return program;
}