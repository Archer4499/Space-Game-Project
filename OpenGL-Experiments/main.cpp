#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define LOG_TYPE_NO 0
#define LOG_TYPE_COUT 1
#define LOG_TYPE_FILE 2

#define LOG_TYPE LOG_TYPE_FILE

#include "logging.h"
#include "config.h"
#include "loadShader.h"

#define LOG_LEVEL INFO
#define LOG_FILE "debug.log"
#define CONFIG_FILE "config.cfg"
#define VERTEX_FILE "Assets/Shaders/shader.vert"
#define FRAGMENT_FILE "Assets/Shaders/shader.frag"


// TODO(Derek): Log more info and errs


void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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
            log("Failed to load config file, using and saving defaults", ERR);
        } else if (err == 2) {
            log("Config file invalid", ERR);
            return -1;
        }
    }
    // saveConfig(&conf, CONFIG_FILE);


    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    int width = std::stoi(conf.data["width"]);
    int height = std::stoi(conf.data["height"]);

    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Experiments", NULL, NULL);
    if (window == NULL) {
        log("Failed to create GLFW window", ERR);
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        log("Failed to initialise GLAD", ERR);
        cleanup();
        return -1;
    }

    // Load shaders
    log("Loading Shaders", INFO);
    int shaderProgram = loadShader(VERTEX_FILE, FRAGMENT_FILE);

    // Window loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    cleanup();
    return 0;
}
