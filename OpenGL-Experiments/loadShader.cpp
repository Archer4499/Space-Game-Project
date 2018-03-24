#include <glad/glad.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "loadShader.h"
#include "logging.h"


std::string readFile(const char *filePath) {
    std::ifstream fs(filePath);

    if(!fs.is_open()) {
        log("Could not read file " + std::string(filePath), ERR);
        return "";
    }

    std::stringstream buffer;
    buffer << fs.rdbuf();

    fs.close();
    return buffer.str();
}


int loadShader(const char *vertexPath, const char *fragmentPath) {
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read shaders
    std::string vertexShaderStr = readFile(vertexPath);
    std::string fragmentShaderStr = readFile(fragmentPath);
    const char *vertexShaderSrc = vertexShaderStr.c_str();
    const char *fragmentShaderSrc = fragmentShaderStr.c_str();

    int success;
    int logLength;
    char infoLog[512];

    // Compile vertex shader
    log("Compiling vertex shader.", INFO);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);

    // Check vertex shader
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    glGetProgramiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1) {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
        if (!success) {
            log("Vertex Shader Compilation Failed" + std::string(infoLog), ERR);
        } else {
            log(infoLog, INFO);
        }
    }

    // Compile fragment shader
    log("Compiling fragment shader.", INFO);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);

    // Check fragment shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    glGetProgramiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1) {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
        if (!success) {
            log("Fragment Shader Compilation Failed" + std::string(infoLog), ERR);
        } else {
            log(infoLog, INFO);
        }
    }


    // Link shaders
    log("Linking program", INFO);
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1) {
        glGetShaderInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        if (!success) {
            log("Shader Program Linking Failed" + std::string(infoLog), ERR);
        } else {
            log(infoLog, INFO);
        }
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
