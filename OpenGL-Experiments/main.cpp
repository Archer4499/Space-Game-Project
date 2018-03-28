#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
// #include <cmath>

// TODO(Derek): Remove requirement for these defines
#define LOG_TYPE_NO 0
#define LOG_TYPE_COUT 1
#define LOG_TYPE_FILE 2

#define LOG_TYPE LOG_TYPE_FILE
#include "logging.h"

#include "config.h"
#include "loadResources.h"

#define LOG_LEVEL INFO
#define LOG_FILE "debug.log"
#define CONFIG_FILE "config.cfg"
#define VERTEX_FILE "Resources/Shaders/shader.vert"
#define FRAGMENT_FILE "Resources/Shaders/shader.frag"
// #define TEXTURE1_FILE "Resources/Textures/container.jpg"
// #define TEXTURE2_FILE "Resources/Textures/awesomeface.png"
// #define TEXTURE_CH_FILE "Resources/Textures/chalet.jpg"
// #define MODEL_CH_FILE "Resources/Models/chalet.obj"
#define TEXTURE_CH_FILE "Resources/Textures/item_box.png"
#define MODEL_CH_FILE "Resources/Models/item_box.obj"


// TODO(Derek): Log more info and errs


void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // gluPerspective(45.0, (float)w / (float)h, 0.01f, 100.0f);
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void cleanup() {
    glfwTerminate();
    logClose();
}


int main(int argc, char const *argv[]) {
    // Start logging
    if (logOpen(LOG_FILE, LOG_LEVEL)) {
        std::cerr << "Log file: \"" << LOG_FILE << "\" could not be opened for writing." << std::endl;
        return -1;
    }

    // Load config file
    Config conf;

    if (int err = loadConfig(&conf, CONFIG_FILE)) {
        if (err == 1) {
            log("Failed to load config file, using and saving defaults: " + std::string(CONFIG_FILE), ERR);
        } else if (err == 2) {
            log("Config file invalid: " + std::string(CONFIG_FILE), ERR);
            cleanup();
            return -1;
        }
    }

    int width = std::stoi(conf.data["width"]);
    int height = std::stoi(conf.data["height"]);
    log("Config file loaded: " + std::string(CONFIG_FILE), INFO);
    // saveConfig(&conf, CONFIG_FILE);
    // log("Config file saved: " + std::string(CONFIG_FILE), INFO);
    // END Load config


    // Init GLFW
    if (!glfwInit()) {
        log("Failed to initialize GLFW.", ERR);
        cleanup();
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Experiments", NULL, NULL);
    if (window == NULL) {
        log("Failed to create GLFW window", ERR);
        cleanup();
        return -1;
    }

    glfwMakeContextCurrent(window);
    // glfwSwapInterval(1); // Vsync

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        log("Failed to initialise GLAD", ERR);
        cleanup();
        return -1;
    }

    log("Loading Shaders", INFO);
    int shaderProgram = loadShader(VERTEX_FILE, FRAGMENT_FILE);


    log("Loading textures", INFO);
    // unsigned int texture1 = loadTexture(TEXTURE1_FILE);
    // unsigned int texture2 = loadTexture(TEXTURE2_FILE);
    unsigned int textureCH = loadTexture(TEXTURE_CH_FILE);


    log("Loading models", INFO);
    unsigned int VBO, VAO, EBO;
    // if (loadModelOld(&VBO, &VAO, &EBO)) {
    if (loadModel(MODEL_CH_FILE, &VBO, &VAO, &EBO)) {
        cleanup();
        return -1;
    }

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    glUseProgram(shaderProgram);

    glUniform1i(glGetUniformLocation(shaderProgram, "textureCH"), 0);
    // glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    // glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);


    log("Main loop", INFO);
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        render();

        // TODO(Derek): move into render function
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureCH);
        // glBindTexture(GL_TEXTURE_2D, texture1);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, texture2);

        // render container
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // TODO(Derek): should use var (length of indices vector) instead of 36
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        // END Section //


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // TODO(Derek): move into cleanup function
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


    cleanup();
    return 0;
}
