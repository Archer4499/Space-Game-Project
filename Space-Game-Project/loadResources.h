#pragma once
#include <glad/glad.h>

#include <vector>
#include <string>
#include <map>

#include "Math\vector.h"

struct Model {
    unsigned int VAO, VBO;
    unsigned int numVertices;
    unsigned int texID;
};

struct RenderObject {
    std::vector<Model> models;

    void draw(unsigned int shaderProgram) {
        glUseProgram(shaderProgram);
        for (Model m : models) {
            // Check whether the texture in each material exists
            if (m.texID > 0) {
                glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m.texID);
            }

            glBindVertexArray(m.VAO);
            glDrawArrays(GL_TRIANGLES, 0, m.numVertices);
        }
    }
};

struct InstanceObject {
    vec2 pos;
    float rot;
    vec2 scale;
    vec3 color;
    unsigned int shaderProgram;
    RenderObject renderObj;

    void draw() {
        renderObj.draw(shaderProgram);
    }
};

int loadShader(const char *vertexPath, const char *fragmentPath, unsigned int &shaderProgram);

int loadTexture(const char *texturePath, unsigned int &texID);

int loadSprite(const char *spritePath, unsigned int &VAO, unsigned int &VBO, unsigned int &numVertices);

int loadAllResources(const char *listPath, std::map<std::string, unsigned int> &shaders, std::vector<InstanceObject> &allObjects);
