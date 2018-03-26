#pragma once

int loadShader(const char *vertexPath, const char *fragmentPath);

unsigned int loadTexture(const char *texturePath);

int loadObject(const char *objectFile, unsigned int *VBO, unsigned int *VAO, unsigned int *EBO);
