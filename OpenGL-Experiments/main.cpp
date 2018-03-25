#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

// TODO(Derek): Remove requirement for these defines
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

    log("Loading Shaders", INFO);
    int shaderProgram = loadShader(VERTEX_FILE, FRAGMENT_FILE);


    // TODO(Derek): move into function return VBO, VAO, EBO?
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // END section //


    log("Main loop", INFO);
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        render();

        // TODO(Derek): move into render function
        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time
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
