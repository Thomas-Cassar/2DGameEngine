#pragma once
#include <string>
#include <unordered_map>

class Shader {
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
    unsigned int createShader(const std::string& verFilePath, const std::string& fragFilePath);

    /**
     * @brief Compiles a shader and returns the ID of the compiled shader
     * 
     * @param type GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
     * @param filepath The path to the shader source
     * @return unsigned int ID of the compiled shader code
     */
    unsigned int compileShader(unsigned int type, const std::string filePath);

    public:
    /**
     * @brief Construct a new Shader object
     * 
     * @param vertexFilePath File path of the vertex shader
     * @param fragmentFilePath File path of the fragment shader
     */
    Shader(const std::string vertexFilePath, const std::string fragmentFilePath);

    /**
     * @brief Destroy the Shader object
     */
    ~Shader();

    /**
     * @brief Binds this shader as the current shader
     */
    void bind();

    /**
     * @brief Unbinds the current shader
     */
    void unbind();

    /**
     * @brief Get int representing the location of this shader uniform
     * 
     * @param name The uniform name 
     * @return int ID used for accessing the uniform
     */
    int getUniformLocation(const std::string& name);

    //******************Uniform setters****************************

    /**
     * @brief Set a Uniform1i
     * 
     * @param name Uniform name
     * @param value Int value to set uniform to
     */
    void setUniform1i(const std::string& name, int value);
    /**
     * @brief Set a Uniform1f
     * 
     * @param name Uniform name
     * @param value Float value to set uniform to
     */
	void setUniform1f(const std::string& name, float value);
    /**
     * @brief Set a Uniform2f
     * 
     * @param name Uniform name
     * @param v0 Float value to set uniform param 1 to
     * @param v1 Float value to set uniform param 2 to
     */
	void setUniform2f(const std::string& name, float v0, float v1);
    /**
     * @brief Set a Uniform3f
     * 
     * @param name Uniform name
     * @param v0 Float value to set uniform param 1 to
     * @param v1 Float value to set uniform param 2 to
     * @param f2 Float value to set uniform param 3 to
     */
	void setUniform3f(const std::string& name, float v0, float v1, float f2);
    /**
     * @brief Set a Uniform4f 
     * 
     * @param name Uniform name
     * @param v0 Float value to set uniform param 1 to
     * @param v1 Float value to set uniform param 2 to
     * @param f2 Float value to set uniform param 3 to
     * @param f3 Float value to set uniform param 4 to
     */
	void setUniform4f(const std::string& name, float v0, float v1, float f2, float f3);

    //****************************************************************

};