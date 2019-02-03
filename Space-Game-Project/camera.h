#pragma once

#include <glad/glad.h>

#include "math/math.h"

enum Camera_Movement {
    NONE     = 0,
    FORWARD  = 1,
    BACKWARD = 2,
    LEFT     = 4,
    RIGHT    = 8
};

// Default camera values
#define YAW         -90.0f
#define PITCH        0.0f
#define SPEED        9.0f
#define FRICTION     5.0f
#define SENSITIVITY  0.05f
#define ZOOM         45.0f


class Camera {
public:
    // Camera Attributes
    vec3 Pos;
    vec3 DPos;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float Friction;
    float MouseSensitivity;
    float Zoom;

    explicit Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) {
        Front = vec3(0.0f, 0.0f, -1.0f);
        MovementSpeed = SPEED;
        Friction = FRICTION;
        MouseSensitivity = SENSITIVITY;
        Zoom = ZOOM;
        Pos = position;
        DPos = vec3(0.0f);
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    // : Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    //     Pos = vec3(posX, posY, posZ);
    //     DPos = 0.0f
    //     WorldUp = vec3(upX, upY, upZ);
    //     Yaw = yaw;
    //     Pitch = pitch;
    //     updateCameraVectors();
    // }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    mat4 GetViewMatrix() {
        return lookAt(Pos, Pos + Front, Up);
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(int direction, float deltaTime) {
        vec3 ddPos(0.0f);

        if (direction & FORWARD)
            ddPos += vec3(Front.x, 0.0f, Front.z);
        if (direction & BACKWARD)
            ddPos -= vec3(Front.x, 0.0f, Front.z);
        if (direction & LEFT)
            ddPos -= Right;
        if (direction & RIGHT)
            ddPos += Right;

        ddPos = normalize(ddPos);

        ddPos *= MovementSpeed;
        ddPos -= Friction*DPos;
        Pos += (0.5f*ddPos*powf(deltaTime, 2) + DPos*deltaTime);
        ddPos *= deltaTime;
        DPos += vec3(ddPos.x, 0.0f, ddPos.z);
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset) {
        // TODO(Derek): care about zoom level (fov)
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // Stop Yaw from reaching large values
        Yaw = std::fmod(Yaw, 360.0f);
        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        Pitch = clamp(Pitch, -85.0f, 85.0f);

        // Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset) {
        Zoom = clamp(Zoom - yoffset, 1.0f, 45.0f);
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors() {
        // Calculate the new Front vector
        vec3 front;
        front.x = cos(radians(Yaw)) * cos(radians(Pitch));
        front.y = sin(radians(Pitch));
        front.z = sin(radians(Yaw)) * cos(radians(Pitch));
        Front = normalize(front);
        Right = normalize(cross(Front, WorldUp));
        Up    = normalize(cross(Right, Front));
    }
};
