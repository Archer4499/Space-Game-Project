#ifdef NDEBUG
// Removes cmd window in non debug builds
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

// todo/note area //
// TODO(Derek): Log more info and errs
// TODO(Derek): hot loading of resource files
// TODO(Derek): Replace operator>> overloads with format_arg then remove define
// TODO(Derek): Give math and logging their own repositories
// TODO(Derek): use relative paths for Additional Include Directories
// TODO(Derek): rename models to meshes
// NOTE: __declspec(deprecated("Message here")) int function() {} for deprecating functions
////////////////////


#include "logging.h"
#include "config.h"
#include "loadResources.h"
#include "camera.h"
#include "math/math.h"

#define LOG_LEVEL DEBUG
#define LOG_MODE "w"
// #define LOG_MODE "a"
#define LOG_FILE "debug.log"
#define CONFIG_FILE "config.cfg"

#define OBJECTS_LIST_FILE "objects.list"

#define VERTEX_FILE "Resources/Shaders/shader.vert"
#define FRAGMENT_FILE "Resources/Shaders/shader.frag"

// #define TEXTURE_FILE "Resources/Textures/awesomeface.png"
#define TEXTURE_FILE "Resources/Textures/container.jpg"
// #define TEXTURE_FILE "Resources/Textures/chalet.jpg"
// #define TEXTURE_FILE "Resources/Textures/item_box.png"


// Globals //
// camera
Camera camera(vec3(0.0f, 0.0f, 3.0f));
float lastX = 0.0f;
float lastY = 0.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrameTime = 0.0f;
/////////////


void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    // TODO(Derek): move camera to new centre?
}

void processInput(GLFWwindow *window) {
    // Meta
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Controls
    // TODO(Derek): Allow key remapping: map(GLFW_KEY_W)
    int input = NONE;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        input |= FORWARD;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        input |= BACKWARD;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        input |= LEFT;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        input |= RIGHT;

    camera.ProcessKeyboard(input, deltaTime);
}

void mouse_callback(GLFWwindow *window, double aXPos, double aYPos) {
    float xPos = static_cast<float>(aXPos);
    float yPos = static_cast<float>(aYPos);
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos; // reversed since y-coordinates go from bottom to top

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
    camera.ProcessMouseScroll(static_cast<float>(yOffset));
}

void window_focus_callback(GLFWwindow *window, int focused) {
    // TODO(Derek): when trying to resize window mouse moves to centre of window
    if (focused) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int shutDown(int exitCode) {
    glfwTerminate();
    LOG(INFO, "Running clean-up");
    LOG(INFO, "Exit code: {}", exitCode);
    logClose();
    return exitCode;
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
            LOG(ERR, "Using and saving default config");
            if (saveConfig(&conf, CONFIG_FILE)) {
                return shutDown(-1);
            }
            LOG(INFO, "Config file saved to: {}", CONFIG_FILE);
        } else if (err == 2) {
            return shutDown(-1);
        }
    }

    int confScreenWidth = conf.getInt("width");
    int confScreenHeight = conf.getInt("height");
    int confFullscreen = conf.getInt("fullscreen");
    LOG(INFO, "Config loaded from: {}", CONFIG_FILE);
    // END Load config


    // Init GLFW
    int success = glfwInit();
    LOG_RETURN(FATAL, !success, shutDown(-1), "Failed to initialize GLFW.");

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
        LOG(INFO, "Windowed mode, width: {}, height: {}", confScreenWidth, confScreenHeight);
    } else if (confFullscreen == 1) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        window = glfwCreateWindow(mode->width, mode->height, "OpenGL Experiments", monitor, NULL);
        LOG(INFO, "Windowed fullscreen mode, width: {}, height: {}", mode->width, mode->height);
    } else if (confFullscreen == 2) {
        LOG(FATAL, "Fullscreen mode not yet implemented");
        return shutDown(-1);
    } else {
        LOG(FATAL, "Fullscreen mode setting invalid");
        return shutDown(-1);
    }
    LOG_RETURN(FATAL, window == NULL, shutDown(-1), "Failed to create GLFW window");

    glfwMakeContextCurrent(window);
    // Vsync
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowFocusCallback(window, window_focus_callback);

    // Capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    LOG_RETURN(FATAL, !success, shutDown(-1), "Failed to initialise GLAD");

    glEnable(GL_DEPTH_TEST);


    LOG(DEBUG, "Loading Shaders");
    unsigned int shaderProgram = glCreateProgram();
    int ret = loadShader(VERTEX_FILE, FRAGMENT_FILE, shaderProgram);
    LOG_RETURN(FATAL, ret, shutDown(-1), "Failed to load shaders");

    LOG(DEBUG, "Loading textures");
    unsigned int atexture;
    loadTexture(TEXTURE_FILE, atexture);

    // Objects
    std::vector<InstanceObject> allObjects;
    ret = loadAllObjects(OBJECTS_LIST_FILE, allObjects);
    LOG_RETURN(FATAL, ret, shutDown(-1), "Failed to load objects");


    glUseProgram(shaderProgram);

    glUniform1i(glGetUniformLocation(shaderProgram, "atexture"), 0);
    // glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    // glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);



    LOG(DEBUG, "Main loop");
    while (!glfwWindowShouldClose(window)) {
        // Only do things if window is focused
        if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
            // update deltaTime
            float currentFrameTime = static_cast<float>(glfwGetTime());
            deltaTime = abs(currentFrameTime - lastFrameTime); // Ensure time doesn't go backwards due to precision
            lastFrameTime = currentFrameTime;
            //

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
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            mat4 projection = perspective(radians(camera.Zoom), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

            mat4 view  = camera.GetViewMatrix();
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
            ////

            // TODO(Derek): move inside render function
            for (InstanceObject obj: allObjects) {
                mat4 model(1.0f);
                model = translate(model, obj.pos);
                model = rotate(model, obj.angle, obj.rot);
                model = scale(model, obj.scale);
                glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
                glBindVertexArray(obj.renderObj.VAO);
                glDrawArrays(GL_TRIANGLES, 0, obj.renderObj.numVertices);
            }


            // glBindVertexArray(VAO);
            // glDrawArrays(GL_TRIANGLES, 0, numVertices);
            // // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            // glBindVertexArray(0);

            glfwPollEvents();
            glfwSwapBuffers(window);
        } else {
            // Time pauses while unfocused
            glfwWaitEvents();
            glfwSetTime(static_cast<double>(lastFrameTime));
        }
    }


    // TODO(Derek): move into cleanup function
    for (InstanceObject obj: allObjects) {
        glDeleteVertexArrays(1, &obj.renderObj.VAO);
        glDeleteBuffers(1, &obj.renderObj.VBO);
        // glDeleteBuffers(1, &obj.EBO);
    }


    return shutDown(0);
}
