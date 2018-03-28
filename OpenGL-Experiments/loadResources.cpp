#include <glad/glad.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "loadResources.h"
#include "logging.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
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
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
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
            log("Shader Program Linking Failed: " + std::string(infoLog), ERR);
        } else {
            log(infoLog, INFO);
        }
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


unsigned int loadTexture(const char *texturePath) {
    unsigned int textureID;
    int texWidth, texHeight, nrChannels;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

    unsigned char *data = stbi_load(texturePath, &texWidth, &texHeight, &nrChannels, 0);
    if (!data) {
        log("Failed to load texture: " + std::string(texturePath), ERR);
        return NULL;
    }
    log("Loaded texture: " + std::string(texturePath) + ", w = " + std::to_string(texWidth) +
        ", h = " + std::to_string(texHeight) + ", channels = " + std::to_string(nrChannels), INFO);

    if (nrChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else {
        log("Texture: " + std::string(texturePath) + " not RGB or RGBA.", ERR);
    }
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    return textureID;
}


int loadObject(const char *objectFile, unsigned int *VBO, unsigned int *VAO, unsigned int *EBO) {
    // Return 0 if success
    // Return 1 if failed to load model file
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(*VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return 0;
}
