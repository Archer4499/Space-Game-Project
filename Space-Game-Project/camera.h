#pragma once

#include "math/math.h"

enum Camera_Movement {
    NONE  = 0,
    UP    = 1,
    DOWN  = 2,
    LEFT  = 4,
    RIGHT = 8
};

// Default camera values
// TODO(Derek): Don't use defines because global (check other places as well)
// TODO(Derek): normalise capitalisation
#define SPEED        600.0f
#define FRICTION     4.0f

#define DEFAULTZOOM  1.0f
#define MINZOOM      0.1f
#define MAXZOOM      2.0f

#define LIM_X 960
#define LIM_Y 540

class Camera {
public:
    // Camera Attributes
    vec2 Pos;
    vec2 DPos;
    // Camera options
    float MovementSpeed;
    float Friction;
    float Zoom;

    explicit Camera(vec2 position = vec2(0.0f)) {
        Pos = position;
        DPos = vec2(0.0f);
        MovementSpeed = SPEED;
        Friction = FRICTION;
        Zoom = DEFAULTZOOM;
    }

    mat4 GetViewMatrix() {
        mat4 view(1.0f);
        view[0][0] = Zoom;
        view[1][1] = Zoom;
        view[3][0] = Pos.x;
        view[3][1] = Pos.y;
        return view;
    }

    void ProcessKeyboard(int direction, float deltaTime) {
        vec2 ddPos(0.0f);

        if (direction & UP)
            ddPos += vec2(0.0f, 1.0f);
        if (direction & DOWN)
            ddPos -= vec2(0.0f, 1.0f);
        if (direction & LEFT)
            ddPos += vec2(1.0f, 0.0f);
        if (direction & RIGHT)
            ddPos -= vec2(1.0f, 0.0f);

        ddPos = normalize(ddPos);

        ddPos *= MovementSpeed;
        ddPos -= Friction*DPos;
        Pos += (0.5f*ddPos*powf(deltaTime, 2) + DPos*deltaTime);
        ddPos *= deltaTime;
        DPos += ddPos;

        clampCamera();
    }

    void ProcessMouseScroll(float yOffset, float xPos, float yPos, float deltaTime) {
        float dZoom = yOffset * 1.2f * deltaTime;

        Zoom += dZoom;

        // TODO(Derek): partial zooms near min/max don't move properly
        if (Zoom < MINZOOM) {
            Zoom = MINZOOM;
        } else if (Zoom > MAXZOOM) {
            Zoom = MAXZOOM;
        } else {
            Pos.x = lerp(Pos.x, Pos.x-xPos, dZoom);
            Pos.y = lerp(Pos.y, Pos.y-yPos, dZoom);
            clampCamera();
        }
    }

private:
    void clampCamera() {
        // Clamping position and removing velocity if clamped
        if (Pos.x <= -LIM_X) {
            Pos.x = -LIM_X;
            DPos.x = 0.0f;
        } else if (Pos.x >= LIM_X) {
            Pos.x = LIM_X;
            DPos.x = 0.0f;
        }
        if (Pos.y <= -LIM_Y) {
            Pos.y = -LIM_Y;
            DPos.y = 0.0f;
        } else if (Pos.y >= LIM_Y) {
            Pos.y = LIM_Y;
            DPos.y = 0.0f;
        }
    }
};
