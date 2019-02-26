#pragma once
#include <glad/glad.h>

#include <vector>
#include <string>
#include <map>

#include "Math\vector.h"

struct Sprite {
    unsigned int VAO, VBO;
    unsigned int numVertices;
};

struct InstanceObject {
    vec2 pos;
    float rot;
    vec2 scale;
    vec3 color;
    unsigned int shaderProgram;
    unsigned int texID;
    Sprite sprite;
};

int loadShader(const char *vertexPath, const char *fragmentPath, unsigned int &shaderProgram);

int loadTexture(const char *texturePath, unsigned int &texID);

int loadSprite(const char *spritePath, unsigned int &VAO, unsigned int &VBO, unsigned int &numVertices);

int loadAllResources(const char *listPath, std::map<std::string, unsigned int> &shaders, std::vector<InstanceObject> &allObjects);
