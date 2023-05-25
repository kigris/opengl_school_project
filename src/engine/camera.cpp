#include "engine/camera.hpp"

Camera::Camera(const glm::vec3 &position, const glm::vec3 &up, float yaw, float pitch)
{
    position_ = position;
    worldUp_ = up;
    yaw_ = yaw;
    pitch_ = pitch;
    fov_ = k_FOV;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{
    position_ = glm::vec3(posX, posY, posZ);
    worldUp_ = glm::vec3(upX, upY, upZ);
    yaw_ = yaw;
    pitch_ = pitch;
    fov_ = k_FOV;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position_, position_ + front_, up_);
}

float Camera::getFOV() const
{
    return fov_;
}

glm::vec3 Camera::getPosition() const
{
    return position_;
}

glm::vec3 Camera::getDirection() const
{
    // We could just return the front vector, which is the direction
    return front_;
    // // Or we could extract the direction from the lookAt matrix
    // // We need to invert it to be in world space, not in camera space
    // glm::mat4 invertedLookAtMatrix = glm::inverse(getViewMatrix());
    // // Get the 3rd component, which is the direction vector
    // glm::vec3 frontDirection = glm::vec3(invertedLookAtMatrix[2]);

    // // Normalize and negate it so it is facing forward and its length is 1
    // frontDirection = glm::normalize(-frontDirection);
    // return frontDirection;
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);

    right_ = glm::normalize(glm::cross(front_, worldUp_));
    up_ = glm::normalize(glm::cross(right_, front_));
}

void Camera::handleKeyboard(Movement direction, float dt)
{
    const float velocity = k_Speed * dt;
    switch (direction)
    {
    case Movement::Forward:
        position_ += front_ * velocity;
        break;
    case Movement::Backward:
        position_ -= front_ * velocity;
        break;
    case Movement::Left:
        position_ -= right_ * velocity;
        break;
    case Movement::Right:
        position_ += right_ * velocity;
        break;
    }
}

void Camera::handleMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    const float xoff = xoffset * k_Sensitivity;
    const float yoff = yoffset * k_Sensitivity;

    yaw_ += xoff;
    pitch_ += yoff;

    if (constrainPitch)
    {
        if (pitch_ > 89.0f)
            pitch_ = 89.0f;
        if (pitch_ < -89.0f)
            pitch_ = -89.0f;
    }

    updateCameraVectors();
}

void Camera::handleMouseScroll(float yoffset)
{
    if (fov_ >= 1.0f && fov_ <= 45.0f)
        fov_ -= yoffset;
    if (fov_ <= 1.0f)
        fov_ = 1.0f;
    if (fov_ >= 45.0f)
        fov_ = 45.0f;
}