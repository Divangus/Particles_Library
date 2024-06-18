#include "ParticleCamera.h"

ParticleCamera::ParticleCamera()
{
    //camera
    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    znear = 0.1f;
    zfar = 1000.0f;

    viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

ParticleCamera::ParticleCamera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float znear, float zfar)
{
    //camera
    this->cameraPos = cameraPos;
    this->cameraFront = cameraFront;
    this->cameraUp = cameraUp;

    this->znear = znear;
    this->zfar = zfar;

    viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

ParticleCamera::ParticleCamera(glm::mat4 viewMatrix)
{
    this->viewMatrix = viewMatrix;

    // Extract the rotation part (3x3 matrix)
    glm::mat3 rotation = glm::mat3(viewMatrix);

    // Extract the translation part (last column of the 4x4 matrix)
    glm::vec3 translation = glm::vec3(viewMatrix[3]);

    // Calculate the camera position
    cameraPos = -glm::transpose(rotation) * translation;

    // Extract the front and up vectors from the rotation matrix
    cameraFront = glm::normalize(glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]));
    cameraUp = glm::normalize(glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]));

    znear = 0.1f;
    zfar = 1000.0f;
}

void ParticleCamera::UpdateCamera(glm::mat4 viewMatrix)
{
    this->viewMatrix = viewMatrix;

    // Extract the rotation part (3x3 matrix)
    glm::mat3 rotation = glm::mat3(viewMatrix);

    // Extract the translation part (last column of the 4x4 matrix)
    glm::vec3 translation = glm::vec3(viewMatrix[3]);

    // Calculate the camera position
    cameraPos = -glm::transpose(rotation) * translation;

    // Extract the front and up vectors from the rotation matrix
    cameraFront = glm::normalize(glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]));
    cameraUp = glm::normalize(glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]));
}

void ParticleCamera::SetCameraPos(glm::vec3 cameraPos)
{
    this->cameraPos = cameraPos;
}

void ParticleCamera::SetCameraFront(glm::vec3 cameraFront)
{
    this->cameraFront = cameraFront;
}

void ParticleCamera::SetCameraUp(glm::vec3 cameraUp)
{
    this->cameraUp = cameraUp;
}

void ParticleCamera::SetCameraPlanes(float nearPlane, float farPlanes)
{
    znear = nearPlane;
    zfar = farPlanes;
}

void ParticleCamera::SetAspectRatio(float displaySizeX, float displaySizeY)
{
    aspectRatio = displaySizeX / displaySizeY;
}

glm::mat4 ParticleCamera::GetViewMat()
{
    viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return viewMatrix;
}

glm::mat4 ParticleCamera::GetProjectionMat(float displaySizeX, float displaySizeY)
{
    float aspectRatio = displaySizeX / displaySizeY;
    projMatrix = glm::perspective(glm::radians(60.0f), aspectRatio, znear, zfar);
    return projMatrix;
}

glm::mat4 ParticleCamera::GetProjectionMat(float aspectRatio)
{
    projMatrix = glm::perspective(glm::radians(60.0f), aspectRatio, znear, zfar);
    return projMatrix;
}

//Set AspectRatio with the SetAspectRatio() method
glm::mat4 ParticleCamera::GetProjectionMat()
{
    projMatrix = glm::perspective(glm::radians(60.0f), aspectRatio, znear, zfar);
    return projMatrix;
}