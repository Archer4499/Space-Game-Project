#pragma once
#include <glad/glad.h>

#include <vector>
#include <string>

#include "Math\vector.h"

struct Material {
    int texID[8]; // -1 is no texture
    // unsigned int ambientTex;
    // unsigned int diffuseTex;
    // unsigned int specularTex;
    // unsigned int specularHighlightTex;
    // unsigned int bumpTex;
    // unsigned int displacementTex;
    // unsigned int alphaTex;
    // unsigned int reflectionTex;
    // vec3 ambient;
    // vec3 diffuse;
    // vec3 specular;
    // vec3 transmittance;
    // vec3 emission;
};

struct Model {
    unsigned int VAO, VBO;
    unsigned int numVertices;
    unsigned int matID;
};

struct RenderObject {
    std::vector<Model> models;
    std::vector<Material> materials;

    void draw(unsigned int shaderProgram) {
        glUseProgram(shaderProgram);
        for (Model m : models) {
            // Check each possible texture in each material
            for (int i = 0; i < 8; ++i) {
                if (materials[m.matID].texID[i] > -1) {
                    glActiveTexture(GL_TEXTURE0 + i);
                    glUniform1i(glGetUniformLocation(shaderProgram, ("tex" + std::to_string(i)).c_str()), i);
                    glBindTexture(GL_TEXTURE_2D, materials[m.matID].texID[i]);
                }
            }

            glBindVertexArray(m.VAO);
            glDrawArrays(GL_TRIANGLES, 0, m.numVertices);
        }
    }
};

struct InstanceObject {
    vec3 pos;
    float angle;
    vec3 rot;
    vec3 scale;
    RenderObject renderObj;

    void draw(unsigned int shaderProgram) {
        renderObj.draw(shaderProgram);
    }
};

int loadShader(const char *vertexPath, const char *fragmentPath, unsigned int &shaderProgram);

int loadTexture(const char *texturePath, unsigned int &texID);

int loadMesh(const char *modelPath, unsigned int &VAO, unsigned int &VBO, unsigned int &numVertices);

int loadModel(const char *modelPath, const char *materialBaseDir, RenderObject &renderObj);

// int loadModelOld(unsigned int *VBO, unsigned int *VAO, unsigned int *EBO);

int loadAllObjects(const char *listPath, std::vector<InstanceObject> &allObjects);
