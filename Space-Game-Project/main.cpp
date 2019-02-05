#ifdef NDEBUG
// Removes cmd window in non debug builds
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <map>

// todo/note area //
// NOTE: __declspec(deprecated("Message here")) int function() {} for deprecating functions
// TODO(Derek): Log more info and errs
// TODO(Derek): hot loading of resource files
// TODO(Derek): Replace operator>> overloads with format_arg then remove define
// TODO(Derek): Give math and logging their own repositories
// TODO(Derek): use relative paths for Additional Include Directories
// TODO(Derek): Use materials in shaders
// TODO(Derek): Use multiple lights
// TODO(Derek): Add VSync to config file
// TODO(Derek): Sort out library path
////////////////////


#include "logging.h"
#include "config.h"
#include "loadResources.h"
#include "camera.h"
#include "math/math.h"

#define BACKGROUND_COLOUR 0.2f, 0.3f, 0.3f, 1.0f

#define LOG_LEVEL DEBUG
#define LOG_MODE "w"
// #define LOG_MODE "a"
#define LOG_FILE "debug.log"
#define CONFIG_FILE "config.cfg"

#define OBJECTS_LIST_FILE "objects.list"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    // GAME_WIN
};

// Globals //
GameState gameState = GAME_ACTIVE;
std::map<std::string, unsigned int> shaders;
std::vector<InstanceObject> allObjects;
GLFWwindow* window = NULL;

// camera
Camera camera(vec3(0.0f, 0.0f, 3.0f));
float lastX = 0.0f;
float lastY = 0.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // Time between current frame and last frame
double lastFrameTime = 0.0;
/////////////


void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    // TODO(Derek): move camera to new centre?
}

void processInput() {
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
    // TODO(Derek): stop camera jerking when refocused
    // TODO(Derek): when trying to resize window mouse moves to centre of window
    if (focused) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void render(mat4 &projection, mat4 &view) {
    glClearColor(BACKGROUND_COLOUR);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto const &shaderProgram : shaders) {
        glUseProgram(shaderProgram.second);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.second, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.second, "view"), 1, GL_FALSE, &view[0][0]);

        if (shaderProgram.first == "general") {
            // TODO(Derek): Give list of colours and locations
            glUniform3f(glGetUniformLocation(shaderProgram.second, "lightColor"), 1.0f, 1.0f, 1.0f);
            glUniform3f(glGetUniformLocation(shaderProgram.second, "lightPos"), 1.0f, 2.0f, 0.0f);
        } else if (shaderProgram.first == "light") {
            // Nothing to do here currently
        } else {
            LOG(SPAM, "Rendering unknown shader: {}", shaderProgram.first);
        }
    }

    for (InstanceObject obj: allObjects) {
        glUseProgram(obj.shaderProgram);

        mat4 model(1.0f);
        model = translate(model, obj.pos);
        model = rotate(model, obj.angle, obj.rot);
        model = scale(model, obj.scale);
        glUniformMatrix4fv(glGetUniformLocation(obj.shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

        obj.draw();
    }
}

int shutDown(int exitCode) {
    LOG(INFO, "Running clean-up");

    for (InstanceObject obj: allObjects) {
        for (Model m : obj.renderObj.models) {
            glDeleteVertexArrays(1, &m.VAO);
            glDeleteBuffers(1, &m.VBO);
            // glDeleteBuffers(1, &obj.EBO);
        }
    }
    glfwTerminate();

    LOG(INFO, "Exit code: {}", exitCode);
    logClose();
    return exitCode;
}


int main(int argc, char const *argv[]) {
    // Setup
    {
        // Start logging
        if (logOpen(LOG_FILE, LOG_MODE, LOG_LEVEL)) return -1;

        // Load config file
        Config conf;

        int err = loadConfig(&conf, CONFIG_FILE);
        if (err == 1) {
            LOG(ERR, "Using and saving default config");
            if (saveConfig(&conf, CONFIG_FILE)) {
                return shutDown(-1);
            }
            LOG(INFO, "Config file saved to: {}", CONFIG_FILE);
        } else if (err == 2) {
            return shutDown(-1);
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


        // TODO(Derek): Set window icon
        // TODO(Derek): Allow to be changed during runtime ( http://www.glfw.org/docs/latest/window_guide.html )
        if (confFullscreen == 0) {
            window = glfwCreateWindow(confScreenWidth, confScreenHeight, "Space Game Project", NULL, NULL);
            LOG(INFO, "Windowed mode, width: {}, height: {}", confScreenWidth, confScreenHeight);
        } else if (confFullscreen == 1) {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);

            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

            window = glfwCreateWindow(mode->width, mode->height, "Space Game Project", monitor, NULL);
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

        glfwSwapInterval(1);  // Vsync

        // Callbacks
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetWindowFocusCallback(window, window_focus_callback);

        // Capture mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        LOG_RETURN(FATAL, !success, shutDown(-1), "Failed to initialise GLAD");

        // glEnable(GL_DEPTH_TEST);

        // Objects
        int ret = loadAllResources(OBJECTS_LIST_FILE, shaders, allObjects);
        LOG_RETURN(FATAL, ret, shutDown(-1), "Failed to load objects");
    }

    LOG(DEBUG, "Main loop");
    while (!glfwWindowShouldClose(window)) {
        // Only do things if window is focused
        if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
            // update deltaTime
            double currentFrameTime = glfwGetTime();
            deltaTime = static_cast<float>(max(currentFrameTime - lastFrameTime, 0.000000001)); // Ensure time doesn't go backwards due to precision
            lastFrameTime = currentFrameTime;
            //

            if (gameState == GAME_ACTIVE) {
                processInput();

                // Camera transformations
                int width, height;
                glfwGetFramebufferSize(window, &width, &height);
                mat4 projection = perspective(radians(camera.Zoom), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
                mat4 view  = camera.GetViewMatrix();
                ////

                render(projection, view);
            } else if (gameState == GAME_MENU) {
                processInput();

            }
            glfwPollEvents();
            glfwSwapBuffers(window);
        } else {
            // Time pauses while unfocused
            glfwWaitEvents();
            glfwSetTime(lastFrameTime);
        }
    }

    return shutDown(0);
}
