#include <camera.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>

Camera::Camera(Camera_Type type, glm::vec3 position, glm::vec3 lookat, float yaw, float pitch, float roll)
{
    Position = position;
    LookAt = glm::normalize(lookat - Position);
    WorldUp = glm::vec3(0, 1, 0);
    Front = LookAt;
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));

    Yaw = glm::radians(yaw);
    Pitch = glm::radians(pitch);
    Roll = glm::radians(roll);

    Zoom = ZOOM;
    Speed = SPEED;

    Type = type;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix(unsigned int scr_width, unsigned int scr_height, float near, float far)
{
    if (Type == PERSP)
    {
        return glm::perspective(glm::radians(this->Zoom), (float)scr_width / (float)scr_height, near, far);
    }
    else
    {
        return glm::ortho(-(float)scr_width / (float)scr_height, (float)scr_width / (float)scr_height, -1.0f, 1.0f, near, far);
    }
}

glm::vec3 Camera::GetPosition()
{
    return this->Position;
}

glm::vec3 Camera::GetFront()
{
    return this->Front;
}

glm::vec3 Camera::GetUp()
{
    return this->Up;
}

glm::vec3 Camera::GetRight()
{
    return this->Right;
}

void Camera::Rotate(Camera_Rotate whirl, Camera_Rotate_Direction dir)
{
    glm::quat quaternion;
    float rotate_rad;
    
    if (whirl == YAW)
    {
        if (dir == CLOCKWISE)
        {
            rotate_rad = -Yaw;
        }
        else
        {
            rotate_rad = Yaw;
        }

        quaternion = glm::quat(rotate_rad, Up);
        glm::vec3 newFront = this->Front * quaternion;
        this->Front = newFront;
    }
    else if (whirl == PITCH)
    {
        if (dir == CLOCKWISE)
        {
            rotate_rad = Pitch;
        }
        else
        {
            rotate_rad = -Pitch;
        }

        quaternion = glm::quat(rotate_rad, Right);
        glm::vec3 newFront = this->Front * quaternion;
        this->Front = newFront;
    }
    else if (whirl == ROLL)
    {
        if (dir == CLOCKWISE)
        {
            rotate_rad = Roll;
        }
        else
        {
            rotate_rad = -Roll;
        }

        quaternion = glm::quat(rotate_rad, Front);
        glm::vec3 newUp = this->Up * quaternion;
        this->Up = newUp;
    }

    this->updateCameraVectors(whirl);
}

void Camera::SetSensitive(const float value)
{
    float rad = value * PI;

    Yaw = rad;
    Pitch = rad;
    Roll = rad;
}

void Camera::updateCameraVectors(Camera_Rotate whirl)
{
    if (whirl == YAW)
    {
        Right = glm::normalize(glm::cross(Front, Up));
    }
    else if (whirl == PITCH)
    {
        Up = glm::normalize(glm::cross(Right, Front));
    }
    else if (whirl == ROLL)
    {
        Right = glm::normalize(glm::cross(Front, Up));
    }
}

void Camera::SetPosition(glm::vec3 pos)
{
    this->Position = pos;
}

void Camera::Move(Camera_Movement movement)
{
    if (movement == FORWARD)
    {
        this->Position += Front * Speed;
    }
    else if (movement == BACKWARD)
    {
        this->Position += -Front * Speed;
    }
    else if (movement == LEFT)
    {
        this->Position += -Right * Speed;
    }
    else if (movement == RIGHT)
    {
        this->Position += Right * Speed;
    }
    else if (movement == UP)
    {
        this->Position += Up * Speed;
    }
    else if (movement == DOWN)
    {
        this->Position += -Up * Speed;
    }
}

void Camera::LookAtNewTargetPos(glm::vec3 targetPos)
{
    this->LookAt = glm::normalize(targetPos - this->Position);
    this->Front = this->LookAt;
    this->Right = glm::normalize(glm::cross(Front, WorldUp));
    this->Up = glm::normalize(glm::cross(Right, Front));
}