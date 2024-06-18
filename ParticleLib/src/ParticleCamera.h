#pragma once
#include "ParticleGlobals.h"

class ParticleCamera
{
public:
    ParticleCamera();

    ParticleCamera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float znear, float zfar);

    ParticleCamera(glm::mat4 viewMatrix);

    void UpdateCamera(glm::mat4 viewMatrix);

    void SetCameraPos(glm::vec3 cameraPos);

    void SetCameraFront(glm::vec3 cameraFront);

    void SetCameraUp(glm::vec3 cameraUp);

    void SetCameraPlanes(float nearPlane, float farPlanes);

    void SetAspectRatio(float displaySizeX, float displaySizeY);

    glm::mat4 GetViewMat();

    glm::mat4 GetProjectionMat(float displaySizeX, float displaySizeY);

    glm::mat4 GetProjectionMat(float aspectRatio);

    //Set AspectRatio with the SetAspectRatio() method
    glm::mat4 GetProjectionMat();

    //camera
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float znear = 0.1f;
    float zfar = 1000.0f;

    float aspectRatio = 1280 / 720;

private:
    glm::mat4 viewMatrix = glm::mat4();
    glm::mat4 projMatrix = glm::mat4();
};