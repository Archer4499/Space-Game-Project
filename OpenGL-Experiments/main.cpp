#ifdef NDEBUG
// Removes cmd window in non debug builds
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// TODO(Derek): Remove requirement for these defines
#define LOG_TYPE_NO 0
#define LOG_TYPE_COUT 1
#define LOG_TYPE_FILE 2

#define LOG_TYPE LOG_TYPE_FILE
#define LOG_STREAM_OVERLOAD
#include "logging.h"

#include "config.h"
#include "loadResources.h"
#include "math/math.h"

#define LOG_LEVEL INFO
#define LOG_MODE "w"
// #define LOG_MODE "a"
#define LOG_FILE "debug.log"
#define CONFIG_FILE "config.cfg"
#define VERTEX_FILE "Resources/Shaders/shader.vert"
#define FRAGMENT_FILE "Resources/Shaders/shader.frag"
// #define TEXTURE_FILE "Resources/Textures/awesomeface.png"
#define TEXTURE_FILE "Resources/Textures/container.jpg"
#define MODEL_FILE "Resources/Models/container.obj"
// #define TEXTURE_FILE "Resources/Textures/chalet.jpg"
// #define MODEL_FILE "Resources/Models/chalet.obj"
// #define TEXTURE_FILE "Resources/Textures/item_box.png"
// #define MODEL_FILE "Resources/Models/item_box.obj"


// todo/note area //
// TODO(Derek): Log more info and errs
// TODO(Derek): hot loading of resource files
// TODO(Derek): Replace operator>> overloads with format_arg then remove define
// TODO(Derek): Give math and logging their own repositories
// NOTE: __declspec(deprecated) for deprecating functions
////////////////////


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
    if (logOpen(LOG_FILE, LOG_MODE, LOG_LEVEL)) {
        return -1;
    }

    // Load config file
    Config conf;

    if (int err = loadConfig(&conf, CONFIG_FILE)) {
        if (err == 1) {
            LOG_F(ERR, "Failed to load config file, using and saving defaults: {}", CONFIG_FILE);
            saveConfig(&conf, CONFIG_FILE);
            LOG_F(INFO, "Config file saved: {}", CONFIG_FILE);
        } else if (err == 2) {
            LOG_F(FATAL, "Config file invalid: {}", CONFIG_FILE);
            cleanup();
            return -1;
        }
    }

    int width = std::stoi(conf.data["width"]);
    int height = std::stoi(conf.data["height"]);
    LOG_F(INFO, "Config loaded: {}", CONFIG_FILE);
    // END Load config


    // Init GLFW
    if (!glfwInit()) {
        LOG_F(FATAL, "Failed to initialize GLFW.");
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
        LOG_F(FATAL, "Failed to create GLFW window");
        cleanup();
        return -1;
    }

    glfwMakeContextCurrent(window);
    // glfwSwapInterval(1); // Vsync

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_F(FATAL, "Failed to initialise GLAD");
        cleanup();
        return -1;
    }

    LOG_F(INFO, "Loading Shaders");
    int shaderProgram = loadShader(VERTEX_FILE, FRAGMENT_FILE);


    LOG_F(INFO, "Loading textures");
    // unsigned int texture1 = loadTexture(TEXTURE1_FILE);
    // unsigned int texture2 = loadTexture(TEXTURE2_FILE);
    unsigned int atexture = loadTexture(TEXTURE_FILE);


    LOG_F(INFO, "Loading models");
    // unsigned int VBO, VAO, EBO;
    unsigned int VAO, VBO, numVertices;
    // if (loadModelOld(&VBO, &VAO, &EBO)) {
    if (loadModel(MODEL_FILE, &VAO, &VBO, &numVertices)) {
        cleanup();
        return -1;
    }

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    glUseProgram(shaderProgram);

    glUniform1i(glGetUniformLocation(shaderProgram, "atexture"), 0);
    // glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    // glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);


    LOG_F(INFO, "Main loop");
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        render();

        // TODO(Derek): move into render function
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atexture);
        // glBindTexture(GL_TEXTURE_2D, texture1);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, texture2);

        // render container
        glUseProgram(shaderProgram);


        mat4 transform(1.0f);
        transform = translate(transform, vec3(0.5f, -0.5f, 0.0f));
        transform = rotate(transform, glfwGetTime(), vec3(0.0f, 0.0f, 1.0f));

        // get matrix's uniform location and set matrix
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &(transform[0].x));


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
        // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // END Section //


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // TODO(Derek): move into cleanup function
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);


    cleanup();
    return 0;
}
