#include "graphics/Shader.hpp"
#include "graphics/Graphics.hpp"

#include <fstream>
#include <iostream>

Shader::Shader(const std::string vertexFilePath, const std::string fragmentFilePath)
    : vertexFilePath(vertexFilePath), fragmentFilePath(fragmentFilePath), shaderID(0)
{
    shaderID = createShader(vertexFilePath, fragmentFilePath);
}

Shader::~Shader() { glCheck(glDeleteProgram(shaderID)); }

unsigned int Shader::createShader(const std::string &verFilePath, const std::string &fragFilePath)
{
    unsigned int program = glCreateProgram();
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, verFilePath);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragFilePath);

    glCheck(glAttachShader(program, vertexShader));
    glCheck(glAttachShader(program, fragmentShader));
    glCheck(glLinkProgram(program));
    glCheck(glValidateProgram(program));

    glCheck(glDeleteShader(vertexShader));
    glCheck(glDeleteShader(fragmentShader));

    return program;
}

unsigned int Shader::compileShader(unsigned int type, const std::string filePath)
{
    std::ifstream file(filePath);
    if (file.fail())
    {
        std::cerr << "Could not find shader file \"" << filePath << "\"" << std::endl;
        engineAssert(false);
    }
    // Get filesize and create string of that size
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    std::string fileBuffer(size, ' ');
    // Read file to string
    file.seekg(0);
    file.read(&fileBuffer[0], size);

    // Compile shader
    unsigned int shadID = glCreateShader(type);
    const char *shaderSrc = fileBuffer.c_str();
    glShaderSource(shadID, 1, &shaderSrc, nullptr);
    glCompileShader(shadID);

    // Check compilation status
    int compileResult;
    glGetShaderiv(shadID, GL_COMPILE_STATUS, &compileResult);
    if (compileResult != GL_TRUE)
    {
        int length;
        glGetShaderiv(shadID, GL_INFO_LOG_LENGTH, &length);
        char *compileMessage = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(shadID, length, &length, compileMessage);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "Vertex" : "fragment") << std::endl;
        std::cout << compileMessage << std::endl;
        glDeleteShader(shadID);
        engineAssert(false);
    }

    file.close();
    return shadID;
}

void Shader::bind() { glCheck(glUseProgram(shaderID)); }

void Shader::unbind() { glCheck(glUseProgram(0)); }

int Shader::getUniformLocation(const std::string &name)
{
    // Check if the uniform has already been cached in the hashmap
    if (uniformLocationCache.find(name) != uniformLocationCache.end())
    {
        return uniformLocationCache[name];
    }
    // Otherwise get the uniform
    glCheck(int location = glGetUniformLocation(shaderID, name.c_str()));

    if (location == -1)
    {
        std::cerr << "Uniform " << name << " does not exist" << std::endl;
        engineAssert(false);
    }

    // Cache location
    uniformLocationCache[name] = location;

    return location;
}

void Shader::setUniform1i(const std::string &name, int value) { glCheck(glUniform1i(getUniformLocation(name), value)); }

void Shader::setUniform1f(const std::string &name, float value)
{
    glCheck(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniform2f(const std::string &name, float v0, float v1)
{
    glCheck(glUniform2f(getUniformLocation(name), v0, v1));
}

void Shader::setUniform3f(const std::string &name, float v0, float v1, float v2)
{
    glCheck(glUniform3f(getUniformLocation(name), v0, v1, v2));
}

void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    glCheck(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &matrix)
{
    glCheck(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}