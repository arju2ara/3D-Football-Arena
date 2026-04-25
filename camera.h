//
//  camera.h
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    P_UP,
    P_DOWN,
    Y_LEFT,
    Y_RIGHT,
    R_LEFT,
    R_RIGHT
};

// Default camera values
const float YAW = 90.0f;
const float PITCH = 0.0f;
const float ROLL = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.01f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::vec3 Target;
    float Distance;
    float Theta;
    float Phi;
    // euler Angles
    float Yaw;
    float Pitch;
    float Roll;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    // Bird's Eye View state
    bool isBirdsEyeView;
    glm::vec3 prevPosition;
    float prevYaw, prevPitch;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float roll = ROLL) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), isBirdsEyeView(false)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        Roll = roll;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), isBirdsEyeView(false)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        //return glm::lookAt(Position, Position - Front, Up);
        glm::mat4 Matrix = glm::mat4(1.0f);


        glm::vec3 X(1.0f, 0.0f, 0.0f);
        glm::vec3 Y(0.0f, 1.0f, 0.0f);
        glm::vec3 Z(0.0f, 0.0f, 1.0f);

        Z = Position - (Position + Front);
        Z = glm::normalize(Z);
        Y = Up;
        X = glm::cross(Y, Z);
        Y = glm::cross(Z, X);
        X = glm::normalize(X);
        Y = glm::normalize(Y);
        Matrix[0][0] = X.x;
        Matrix[1][0] = X.y;
        Matrix[2][0] = X.z;
        Matrix[3][0] = -glm::dot(X, Position);

        Matrix[0][1] = Y.x;
        Matrix[1][1] = Y.y;
        Matrix[2][1] = Y.z;
        Matrix[3][1] = -glm::dot(Y, Position);

        Matrix[0][2] = Z.x;
        Matrix[1][2] = Z.y;
        Matrix[2][2] = Z.z;
        Matrix[3][2] = -glm::dot(Z, Position);


        Matrix[0][3] = 0.0f;
        Matrix[1][3] = 0.0f;
        Matrix[2][3] = 0.0f;
        Matrix[3][3] = 1.0f;
        //Matrix = glm::transpose(Matrix);
        return Matrix;
    }
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position -= Front * velocity;
        if (direction == BACKWARD)
            Position += Front * velocity;
        if (direction == LEFT)
            Position += Right * velocity;
        if (direction == RIGHT)
            Position -= Right * velocity;
        if (direction == UP)
            Position += Up * velocity;
        if (direction == DOWN)
            Position -= Up * velocity;
        if (direction == P_UP)
            Pitch += velocity * 10;
        if (direction == P_DOWN)
            Pitch -= velocity * 10;
        if (direction == Y_LEFT)
            Yaw += velocity * 10;
        if (direction == Y_RIGHT)
            Yaw -= velocity * 10;
        if (direction == R_LEFT)
            Roll += velocity * 10;
        if (direction == R_RIGHT)
            Roll -= velocity * 10;
        updateCameraVectors();
    }

    // Implementing Bird's Eye View toggle
    void toggleBirdsEyeView() {
        if (isBirdsEyeView) {
            // Restore previous position and orientation
            Position = prevPosition;
            Yaw = prevYaw;
            Pitch = prevPitch;
        }
        else {
            // Store the current position and orientation before switching to Bird's Eye View
            prevPosition = Position;
            prevYaw = Yaw;
            prevPitch = Pitch;
            setBirdsEyeView();
        }

        isBirdsEyeView = !isBirdsEyeView;

        // Force an immediate update of the camera vectors
        updateCameraVectors();

        // Force a refresh to immediately apply the changes
        glfwPostEmptyEvent(); // This triggers a refresh in the GLFW window
    }

    // Implementing Bird's Eye View
    void setBirdsEyeView() {
        Position = glm::vec3(0.0f, 3.1f, 0.0f);  // Set the position above the scene
        Yaw = 0.0f;  // Looking downward
        Pitch = -90.0f;  // Looking straight down
        updateCameraVectors();  // Recalculate vectors
    }


    // Implementing rotating the camera around a look-at point
    void rotateAroundTarget(float dTheta, float dPhi) {
        glm::vec3 direction = Position - Target;  // Vector from the target
        float radius = glm::length(direction);
        Theta += dTheta;
        Phi = glm::clamp(Phi + dPhi, 0.1f, glm::radians(179.9f));  // Avoid gimbal lock

        // Update camera position
        Position.x = Target.x + radius * sin(Theta) * cos(Phi);
        Position.y = Target.y + radius * sin(Phi);
        Position.z = Target.z + radius * cos(Theta) * cos(Phi);
        updateCameraVectors();
    }

    void setPosition(const glm::vec3& position) {
        Position = position;
    }

    void setDirection(const glm::vec3& direction) {
        Front = glm::normalize(direction);
        Right = glm::normalize(glm::cross(Front, glm::vec3(0.0f, 1.0f, 0.0f)));
        Up = glm::normalize(glm::cross(Right, Front));
    }



    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        glm::mat4 rotationMatrix;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
        if (Roll != 0.0f) {
            rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(Roll), Front);
            Up = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(Up, 0.0f)));
            Right = glm::normalize(glm::cross(Front, Up));
        }
    }
};
#endif
#pragma once