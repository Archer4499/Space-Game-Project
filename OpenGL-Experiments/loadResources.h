#pragma once
#include <vector>
#include "Math\vector.h"

int loadShader(const char *vertexPath, const char *fragmentPath);

unsigned int loadTexture(const char *texturePath);

int loadModel(const char *modelPath, unsigned int *VAO, unsigned int *VBO, unsigned int *numVertices);

int loadModelOld(unsigned int *VBO, unsigned int *VAO, unsigned int *EBO);

struct RenderObject {
    unsigned int VAO, VBO, numVertices;
    unsigned int texID;
};

struct InstanceObject {
    vec3 pos;
    float angle;
    vec3 rot;
    vec3 scale;
    RenderObject renderObj;
};

int loadAllObjects(const char *listPath, std::vector<InstanceObject> &allObjects);
