#pragma once

int loadShader(const char *vertexPath, const char *fragmentPath);

unsigned int loadTexture(const char *texturePath);

int loadModel(const char *modelPath, unsigned int *VBO, unsigned int *VAO, unsigned int *EBO);

int loadModelOld(unsigned int *VBO, unsigned int *VAO, unsigned int *EBO);
