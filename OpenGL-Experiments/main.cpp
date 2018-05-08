#ifdef NDEBUG
// Removes cmd window in non debug builds
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// todo/note area //
// TODO(Derek): Log more info and errs
// TODO(Derek): hot loading of resource files
// TODO(Derek): Replace operator>> overloads with format_arg then remove define
// TODO(Derek): Give math and logging their own repositories
// TODO(Derek): use relative paths for Additional Include Directories
// NOTE: __declspec(deprecated) for deprecating functions
////////////////////


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




void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void cleanup() {
    glfwTerminate();
    LOG_F(INFO, "Clean-up");
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

    int confScreenWidth = conf.getInt("width");
    int confScreenHeight = conf.getInt("height");
    bool confFullscreen = conf.getInt("fullscreen");
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

    GLFWwindow* window = NULL;

    // TODO(Derek): Set window icon
    // TODO(Derek): Allow to be changed during runtime ( http://www.glfw.org/docs/latest/window_guide.html )
    if (confFullscreen == 0) {
        window = glfwCreateWindow(confScreenWidth, confScreenHeight, "OpenGL Experiments", NULL, NULL);
        LOG_F(INFO, "Windowed mode enabled");
    } else if (confFullscreen == 1) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        window = glfwCreateWindow(mode->width, mode->height, "OpenGL Experiments", monitor, NULL);
        LOG_F(INFO, "Windowed fullscreen mode enabled");
    } else if (confFullscreen == 2) {
        LOG_F(FATAL, "Fullscreen mode not yet implemented");
        cleanup();
        return -1;
    } else {
        LOG_F(FATAL, "Fullscreen mode setting invalid");
        cleanup();
        return -1;
    }
    if (window == NULL) {
        LOG_F(FATAL, "Failed to create GLFW window");
        cleanup();
        return -1;
    }

    glfwMakeContextCurrent(window);
    // Vsync
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_F(FATAL, "Failed to initialise GLAD");
        cleanup();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    LOG_F(INFO, "Loading Shaders");
    int shaderProgram = loadShader(VERTEX_FILE, FRAGMENT_FILE);


    LOG_F(INFO, "Loading textures");
    unsigned int atexture = loadTexture(TEXTURE_FILE);


    LOG_F(INFO, "Loading models");
    unsigned int VAO, VBO, numVertices;
    if (loadModel(MODEL_FILE, &VAO, &VBO, &numVertices)) {
        cleanup();
        return -1;
    }

    glUseProgram(shaderProgram);

    glUniform1i(glGetUniformLocation(shaderProgram, "atexture"), 0);
    // glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    // glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);

    // Set projection transform
    mat4 projection(1.0f);
    projection = perspective(radians(45.0f), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);


    LOG_F(INFO, "Main loop");
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        render();

        // TODO(Derek): move into render function somehow
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atexture);
        // glBindTexture(GL_TEXTURE_2D, texture1);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, texture2);

        glUseProgram(shaderProgram);

        // Transformations
        mat4 model(1.0f), view(1.0f);
        model = rotate(model, static_cast<float>(glfwGetTime()), vec3(0.5f, 1.0f, 0.0f));
        // view  = translate(view, vec3(0.0f, 0.0f, -3.0f));
        view  = lookAt(vec3(0.0f, 0.0f, 3.0f),
                       vec3(0.0f, 0.0f, 0.0f),
                       vec3(0.0f, 1.0f, 0.0f));

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc  = glGetUniformLocation(shaderProgram, "view");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        ////

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


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
