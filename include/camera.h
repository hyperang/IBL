#ifndef _camera_h_
#define _camera_h_

#include <glad/glad.h>
#include <glm/glm.hpp>

const float PI = 3.14159265359;

enum Camera_Type
{
    PERSP,
    ORTHO
};

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum Camera_Rotate
{
    YAW,
    PITCH,
    ROLL
};

enum Camera_Rotate_Direction
{
    CLOCKWISE,
    COUNTERCLOCKWISE
};

// Default camera values
const float ZOOM = 45.0f;
const float SPEED = 1.0f;

class Camera
{
public:
    Camera(Camera_Type type, glm::vec3 position, glm::vec3 lookat, float yaw, float pitch, float roll);

    glm::mat4 GetViewMatrix();

    glm::mat4 GetProjectionMatrix(unsigned int scr_width, unsigned int scr_height, float near, float far);

    void Rotate(Camera_Rotate whirl, Camera_Rotate_Direction dir);

    void Move(Camera_Movement movement);

    void SetSensitive(const float value);

    void SetPosition(glm::vec3 pos);

    // void SetFront(glm::vec3 dir);

    glm::vec3 GetPosition();

    glm::vec3 GetFront();

    glm::vec3 GetUp();

    glm::vec3 GetRight();

    void LookAtNewTargetPos(glm::vec3 targetPos);

protected:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 LookAt;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    float Roll;
    // camera options
    float Zoom;
    float Speed;

    Camera_Type Type;

private:
    void updateCameraVectors(Camera_Rotate whirl);
};

#endif // !_camera_h_
