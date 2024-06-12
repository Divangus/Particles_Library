#include "Camera.h"

Camera::Camera()
{
    //camera
    cameraPos = vec3(0.0f, 0.0f, 3.0f);
    cameraFront = vec3(0.0f, 0.0f, -1.0f);
    cameraUp = vec3(0.0f, 1.0f, 0.0f);

    znear = 0.1f;
    zfar = 1000.0f;

    viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

Camera::Camera(vec3 cameraPos, vec3 cameraFront, vec3 cameraUp, float znear, float zfar)
{
    //camera
    this->cameraPos = cameraPos;
    this->cameraFront = cameraFront;
    this->cameraUp = cameraUp;

    this->znear = znear;
    this->zfar = zfar;

    viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::SetCameraPos(vec3 cameraPos)
{
    this->cameraPos = cameraPos;
}

void Camera::SetCameraFront(vec3 cameraFront)
{
    this->cameraFront = cameraFront;
}

void Camera::SetCameraUp(vec3 cameraUp)
{
    this->cameraUp = cameraUp;
}

void Camera::SetCameraPlanes(float nearPlane, float farPlanes)
{
    znear = nearPlane;
    zfar = farPlanes;
}

void Camera::SetAspectRatio(float displaySizeX, float displaySizeY)
{
    aspectRatio = displaySizeX / displaySizeY;
}

mat4 Camera::GetViewMat()
{
    viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return viewMatrix;
}

mat4 Camera::GetProjectionMat(float displaySizeX, float displaySizeY)
{
    float aspectRatio = displaySizeX / displaySizeY;
    projMatrix = glm::perspective(glm::radians(60.0f), aspectRatio, znear, zfar);
    return projMatrix;
}

mat4 Camera::GetProjectionMat(float aspectRatio)
{
    projMatrix = glm::perspective(glm::radians(60.0f), aspectRatio, znear, zfar);
    return projMatrix;
}

//Set AspectRatio with the SetAspectRatio() method
mat4 Camera::GetProjectionMat()
{
    projMatrix = glm::perspective(glm::radians(60.0f), aspectRatio, znear, zfar);
    return projMatrix;
}