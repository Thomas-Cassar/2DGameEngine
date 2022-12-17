module;
#include "glm/glm.hpp"
#include "graphics/Common.hpp"
#include <fstream>
#include <string>
#include <unordered_map>
export module Graphics:Shader;

namespace Graphics
{
export class Shader {
private:
    /** File path of the vertex shader*/
    std::string vertexFilePath;
    /** File path of the fragment shader*/
    std::string fragmentFilePath;
    /** ID given to the shader by OpenGL*/
    unsigned int shaderID;
    /** Cache for the locations of the uniforms of the shader*/
    std::unordered_map<std::string, int> uniformLocationCache;

    /**
     * @brief Create a Shader object
     *
     * @param verFilePath File path to the GLSL vertex shader
     * @param fragFilePath File path to the GLSL fragment shader
     * @return unsigned int The created shader program
     */
    unsigned int createShader(const std::string& verFilePath, const std::string& fragFilePath)
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

    /**
     * @brief Compiles a shader and returns the ID of the compiled shader
     *
     * @param type GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
     * @param filepath The path to the shader source
     * @return unsigned int ID of the compiled shader code
     */
    unsigned int compileShader(unsigned int type, const std::string filePath)
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
        const char* shaderSrc = fileBuffer.c_str();
        glShaderSource(shadID, 1, &shaderSrc, nullptr);
        glCompileShader(shadID);

        // Check compilation status
        int compileResult;
        glGetShaderiv(shadID, GL_COMPILE_STATUS, &compileResult);
        if (compileResult != GL_TRUE)
        {
            int length;
            glGetShaderiv(shadID, GL_INFO_LOG_LENGTH, &length);
            char* compileMessage = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(shadID, length, &length, compileMessage);
            std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "Vertex" : "fragment") << std::endl;
            std::cout << compileMessage << std::endl;
            glDeleteShader(shadID);
            engineAssert(false);
        }

        file.close();
        return shadID;
    }

public:
    /**
     * @brief Construct a new Shader object
     *
     * @param vertexFilePath File path of the vertex shader
     * @param fragmentFilePath File path of the fragment shader
     */
    Shader(const std::string vertexFilePath, const std::string fragmentFilePath)
        : vertexFilePath(vertexFilePath), fragmentFilePath(fragmentFilePath), shaderID(0)
    {
        shaderID = createShader(vertexFilePath, fragmentFilePath);
    }

    /**
     * @brief Destroy the Shader object
     */
    ~Shader() { glCheck(glDeleteProgram(shaderID)); }

    /**
     * @brief Binds this shader as the current shader
     */
    void bind() { glCheck(glUseProgram(shaderID)); }

    /**
     * @brief Unbinds the current shader
     */
    void unbind() { glCheck(glUseProgram(0)); }

    /**
     * @brief Get int representing the location of this shader uniform
     *
     * @param name The uniform name
     * @return int ID used for accessing the uniform
     */
    int getUniformLocation(const std::string& name)
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

    //******************Uniform setters****************************

    /**
     * @brief Set a Uniform1i
     *
     * @param name Uniform name
     * @param value Int value to set uniform to
     */
    void setUniform1i(const std::string& name, int value) { glCheck(glUniform1i(getUniformLocation(name), value)); }

    /**
     * @brief Set a Uniform1f
     *
     * @param name Uniform name
     * @param value Float value to set uniform to
     */
    void setUniform1f(const std::string& name, float value) { glCheck(glUniform1f(getUniformLocation(name), value)); }

    /**
     * @brief Set a Uniform2f
     *
     * @param name Uniform name
     * @param v0 Float value to set uniform param 1 to
     * @param v1 Float value to set uniform param 2 to
     */
    void setUniform2f(const std::string& name, float v0, float v1)
    {
        glCheck(glUniform2f(getUniformLocation(name), v0, v1));
    }

    /**
     * @brief Set a Uniform3f
     *
     * @param name Uniform name
     * @param v0 Float value to set uniform param 1 to
     * @param v1 Float value to set uniform param 2 to
     * @param f2 Float value to set uniform param 3 to
     */
    void setUniform3f(const std::string& name, float v0, float v1, float v2)
    {
        glCheck(glUniform3f(getUniformLocation(name), v0, v1, v2));
    }

    /**
     * @brief Set a Uniform4f
     *
     * @param name Uniform name
     * @param v0 Float value to set uniform param 1 to
     * @param v1 Float value to set uniform param 2 to
     * @param f2 Float value to set uniform param 3 to
     * @param f3 Float value to set uniform param 4 to
     */
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
    {
        glCheck(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
    }

    /**
     * @brief Set the Uniform Mat 4f
     *
     * @param name Uniform name
     * @param matrix Matrix to set uniform to
     */
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
    {
        glCheck(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
    }

    //****************************************************************
};
} // namespace Graphics