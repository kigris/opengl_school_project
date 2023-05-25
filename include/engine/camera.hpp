#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float k_Yaw = -90.0f;
const float k_Pitch = 0.0f;
const float k_Speed = 2.5f;
const float k_Sensitivity = 0.1f;
const float k_FOV = 45.0f;

class Camera {
public:
    enum class Movement {
        Forward = 0,
        Backward = 1,
        Left = 2,
        Right = 3,
    };

    Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = k_Yaw, float pitch = k_Pitch);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,  float yaw, float pitch);

    [[nodiscard]] glm::mat4 getViewMatrix() const;
    [[nodiscard]] float getFOV() const;
    [[nodiscard]] glm::vec3 getPosition() const;
    [[nodiscard]] glm::vec3 getDirection() const;

    void handleKeyboard(Movement direction, float dt);
    void handleMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void handleMouseScroll(float yoffset);

private:
    void updateCameraVectors();

private:
    glm::vec3 position_, front_, up_, right_, worldUp_;
    float yaw_, pitch_;
    float fov_;
};

#endif
