#include "Camera.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp)
        : cameraPosition(cameraPosition), cameraTarget(cameraTarget), cameraUpDirection(cameraUp) {
        cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUp));
        cameraUpDirection = glm::cross(cameraRightDirection, cameraFrontDirection);
    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        //TODO

        return glm::lookAt(cameraPosition, cameraPosition + cameraFrontDirection, cameraUpDirection);
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        //TODO
        switch (direction) {
        case MOVE_FORWARD:
            cameraPosition += cameraFrontDirection * speed;
            break;
        case MOVE_BACKWARD:
            cameraPosition -= cameraFrontDirection * speed;
            break;
        case MOVE_RIGHT:
            cameraPosition += cameraRightDirection * speed;
            break;
        case MOVE_LEFT:
            cameraPosition -= cameraRightDirection * speed;
            break;
        }
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitchOffset, float yawOffset) {
        static float pitch = 0.0f;
        static float yaw = -90.0f;  // Starting direction

        pitch += pitchOffset;
        yaw += yawOffset;

        // Clamp pitch to avoid flipping the camera view
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        // Calculate the new front direction with both yaw and pitch
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFrontDirection = glm::normalize(front);

        // Recalculate right and up vectors, with up direction fixed to avoid tilt
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
        cameraUpDirection = glm::cross(cameraRightDirection, cameraFrontDirection);
    }

    void Camera::setCameraPosition(glm::vec3 newPosition) {
        cameraPosition = newPosition;
        cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        cameraRightDirection = glm::normalize(glm::cross(cameraUpDirection, cameraFrontDirection));
        cameraUpDirection = glm::cross(cameraFrontDirection, cameraRightDirection);
    }

    void Camera::setCameraTarget(glm::vec3 newTarget) {
        cameraTarget = newTarget;
        cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        cameraRightDirection = glm::normalize(glm::cross(cameraUpDirection, cameraFrontDirection));
        cameraUpDirection = glm::cross(cameraFrontDirection, cameraRightDirection);
    }
}