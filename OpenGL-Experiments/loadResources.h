#pragma once
#include <vector>
#include "Math\vector.h"

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

int loadShader(const char *vertexPath, const char *fragmentPath, unsigned int &shaderProgram);

int loadTexture(const char *texturePath, unsigned int &texID);

int loadMesh(const char *modelPath, unsigned int &VAO, unsigned int &VBO, unsigned int &numVertices);

int loadModel(const char *modelPath, const char *materialBaseDir, RenderObject &renderObj);

// int loadModelOld(unsigned int *VBO, unsigned int *VAO, unsigned int *EBO);

int loadAllObjects(const char *listPath, std::vector<InstanceObject> &allObjects);
