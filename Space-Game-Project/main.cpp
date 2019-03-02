#ifndef _DEBUG
// Enable NDEBUG for Visual Studio
#define NDEBUG
#endif
#ifdef NDEBUG
// Removes cmd window in non debug builds
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <map>

// Notes //
// NOTE: __declspec(deprecated("Message here")) int function() {} for deprecating functions
// NOTE: (0,0) is at top left corner
// NOTE: Easiest way to add sprites is to use https://threejs.org/editor/ and then 'export OBJ" then run convertToSprite.py
// To-dos //
// TODO(Derek): Make Icon
// TODO(Derek): hot loading of resource files
// TODO(Derek): Replace operator>> overloads with format_arg then remove define
// TODO(Derek): Give math and logging their own repositories
// TODO(Derek): remove light shaders
// TODO(Derek): Method of having sprite shapes with different textures
// TODO(Derek): remove tiny_obj_loader.h
// TODO(Derek): Add moveToward for vectors lerping
// Decisions //
// TODO(Derek): decide whether resolution changes zoom or scaling (http://www.david-amador.com/2013/04/opengl-2d-independent-resolution-rendering/)
// TODO(Derek): possibly lock aspect ratio using glfwSetWindowAspectRatio(window, 16, 9);
///////////////


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

int lastKeyState[GLFW_KEY_LAST];

// // camera
Camera camera(vec2(0.0f));
// float lastX = 0.0f;
// float lastY = 0.0f;
// bool firstMouse = true;

// timing
float deltaTime = 0.0f; // Time between current frame and last frame
double lastFrameTime = 0.0;
float timeFactor = 1.0f;

// Menu
unsigned int menuSelected = 0;
#define NUM_MENU_OPTIONS 1
/////////////


void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    int size[4];
    // int oldWidth, oldHeight;
    glGetIntegerv(GL_VIEWPORT, size);
    // glfwGetFramebufferSize(window, &oldWidth, &oldHeight);
    glViewport(0, 0, width, height);
    camera.Pos.x += (width - size[2]) / 2.0f;
    camera.Pos.y += (height - size[3]) / 2.0f;
    // TODO(Derek): move camera to new centre?
}

bool keyPressed(int key) {
    if (glfwGetKey(window, key) != lastKeyState[key] && lastKeyState[key] == GLFW_RELEASE)
        return true;
    else
        return false;
}
bool keyReleased(int key) {
    if (glfwGetKey(window, key) != lastKeyState[key] && lastKeyState[key] == GLFW_PRESS)
        return true;
    else
        return false;
}

void processInput() {
    if (gameState == GAME_MENU) {
        if (keyReleased(GLFW_KEY_Q))
            glfwSetWindowShouldClose(window, true);

        if (keyReleased(GLFW_KEY_ESCAPE))
            gameState = GAME_ACTIVE;

        if (keyReleased(GLFW_KEY_UP) || keyReleased(GLFW_KEY_W)) {
            if (menuSelected > 0) {
                --menuSelected;
            } else {
                menuSelected = NUM_MENU_OPTIONS - 1;
            }
        }
        if (keyReleased(GLFW_KEY_DOWN) || keyReleased(GLFW_KEY_S))
            menuSelected = (menuSelected + 1) % NUM_MENU_OPTIONS;
    } else if (gameState == GAME_ACTIVE) {
        if (keyReleased(GLFW_KEY_ESCAPE)) {
            menuSelected = 0;
            gameState = GAME_MENU;
        }

        if (keyReleased(GLFW_KEY_LEFT_BRACKET)) {
            timeFactor /= 2;
            LOG(DEBUG, "Slowing to time factor: {}", timeFactor);
        }
        if (keyReleased(GLFW_KEY_RIGHT_BRACKET)) {
            timeFactor *= 2;
            LOG(DEBUG, "Speeding up to time factor: {}", timeFactor);
        }
        if (keyReleased(GLFW_KEY_BACKSLASH)) {
            timeFactor = 1.0f;
            LOG(DEBUG, "Resetting time factor to: {}", timeFactor);
        }

        // Controls
        // TODO(Derek): Allow key remapping: map(GLFW_KEY_W)
        int input = NONE;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            input |= UP;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            input |= DOWN;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            input |= LEFT;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            input |= RIGHT;

        camera.ProcessKeyboard(input, deltaTime);
    }

    for (int i = 0 ; i < GLFW_KEY_LAST ; ++i) {
        // TODO(Derek): possibly change to just updating the list of keys we use
        lastKeyState[i] = glfwGetKey(window, i);
    }
}

void mouse_callback(GLFWwindow *window, double aXPos, double aYPos) {
    // float xPos = static_cast<float>(aXPos);
    // float yPos = static_cast<float>(aYPos);
    // if (firstMouse) {
    //     lastX = xPos;
    //     lastY = yPos;
    //     firstMouse = false;
    // }

    // float xOffset = xPos - lastX;
    // float yOffset = lastY - yPos; // reversed since y-coordinates go from bottom to top

    // lastX = xPos;
    // lastY = yPos;

    // camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
    int width, height;
    double xMPos, yMPos;
    vec2 mPos;

    glfwGetFramebufferSize(window, &width, &height);
    glfwGetCursorPos(window, &xMPos, &yMPos);
    mPos = vec2(static_cast<float>(xMPos), static_cast<float>(yMPos));

    camera.ProcessMouseScroll(static_cast<float>(yOffset), mPos, deltaTime);
    LOG(DEBUG, "Zoom level now: {}", camera.Zoom);
}

void window_focus_callback(GLFWwindow *window, int focused) {
    // TODO(Derek): when trying to resize window mouse moves to centre of window
    if (focused) {
        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void renderGame() {
    // Camera transformations
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    mat4 projection = ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
    mat4 view  = camera.GetViewMatrix();
    ////

    glClearColor(BACKGROUND_COLOUR);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto const &shaderProgram : shaders) {
        glUseProgram(shaderProgram.second);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.second, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.second, "view"), 1, GL_FALSE, &view[0][0]);

        if (shaderProgram.first == "general") {
            // TODO(Derek): Give list of colours and locations
            // glUniform3f(glGetUniformLocation(shaderProgram.second, "lightColor"), 1.0f, 1.0f, 1.0f);
            // glUniform3f(glGetUniformLocation(shaderProgram.second, "lightPos"), 1.0f, 2.0f, 0.0f);
        } else if (shaderProgram.first == "light") {
            // Nothing to do here currently
        } else {
            LOG(SPAM, "Rendering unknown shader: {}", shaderProgram.first);
        }
    }

    for (InstanceObject obj: allObjects) {
        glUseProgram(obj.shaderProgram);

        mat4 model(1.0f);
        model = translate(model, vec3(obj.pos.x, obj.pos.y, 0.0f));
        model = rotate(model, radians(obj.rot), vec3(0.0f, 0.0f, 1.0f));
        model = scale(model, vec3(obj.scale.x, obj.scale.y, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(obj.shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

        glUniform4fv(glGetUniformLocation(obj.shaderProgram, "spriteColor"), 1, &obj.color[0]);

        if (obj.texID > 0) {
            glUniform1i(glGetUniformLocation(obj.shaderProgram, "tex"), 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, obj.texID);
        }

        glBindVertexArray(obj.sprite.VAO);
        glDrawArrays(GL_TRIANGLES, 0, obj.sprite.numVertices);
    }
}

void renderMenu() {
    // glClearColor(BACKGROUND_COLOUR);
    // glClear(GL_COLOR_BUFFER_BIT);
}

int shutDown(int exitCode) {
    LOG(INFO, "Running clean-up");

    // TODO(Derek): get list of sprites not list of instanceObjects
    for (InstanceObject obj: allObjects) {
        glDeleteVertexArrays(1, &obj.sprite.VAO);
        glDeleteBuffers(1, &obj.sprite.VBO);
        // glDeleteBuffers(1, &obj.sprite.EBO);
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
        bool confVSync = conf.getBool("vsync");
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

        glfwSwapInterval(confVSync);  // Vsync

        // Callbacks
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        // glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetWindowFocusCallback(window, window_focus_callback);

        // // Capture mouse
        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // GLAD
        success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        LOG_RETURN(FATAL, !success, shutDown(-1), "Failed to initialise GLAD");

        // OpenGL options
        // glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
            deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
            deltaTime *= timeFactor;
            lastFrameTime = currentFrameTime;
            //

            glfwPollEvents();
            processInput();

            if (gameState == GAME_ACTIVE) {
                renderGame();
            } else if (gameState == GAME_MENU) {
                renderGame();
                renderMenu();
            }
            glfwSwapBuffers(window);
        } else {
            // Time pauses while unfocused
            glfwWaitEvents();
            glfwSetTime(lastFrameTime);
        }
    }

    return shutDown(0);
}
