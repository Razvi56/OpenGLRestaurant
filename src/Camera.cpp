#include "Camera.hpp"

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraFrontDirection, cameraUp));
        this->cameraUpDirection = glm::cross(this->cameraRightDirection, this->cameraFrontDirection);
    }



    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraPosition + cameraFrontDirection, cameraUpDirection);
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        glm::vec3 directions[] = {cameraFrontDirection, -cameraFrontDirection, cameraRightDirection,
                                  -cameraRightDirection};
        cameraPosition += directions[(int) direction] * speed;
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        glm::mat4 pitchRotation = glm::rotate(pitch, cameraRightDirection);
        glm::mat4 yawRotation = glm::rotate(yaw, cameraUpDirection);

        glm::vec3 cameraFrontAfterPitch = glm::vec3(pitchRotation * glm::vec4(cameraFrontDirection, 0.0f));
        glm::vec3 cameraRightAfterPitch = glm::cross(cameraFrontAfterPitch, glm::vec3(0.0f, 1.0f, 0.0f));
        if (glm::dot(cameraRightAfterPitch, cameraRightDirection) < 0.1f) return;
        cameraFrontDirection = glm::normalize(glm::vec3(yawRotation * glm::vec4(cameraFrontAfterPitch, 0.0f)));
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
        cameraUpDirection = glm::cross(cameraRightDirection, cameraFrontDirection);
    }

    const glm::vec3 &Camera::getCameraPosition() const {
        return cameraPosition;
    }
}
