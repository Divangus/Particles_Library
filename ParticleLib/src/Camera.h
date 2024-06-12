#pragma once
#include "Globals.h"

class Camera
{
public:
    Camera();

    Camera(vec3 cameraPos, vec3 cameraFront, vec3 cameraUp, float znear, float zfar);

    void SetCameraPos(vec3 cameraPos);

    void SetCameraFront(vec3 cameraFront);

    void SetCameraUp(vec3 cameraUp);

    void SetCameraPlanes(float nearPlane, float farPlanes);

    void SetAspectRatio(float displaySizeX, float displaySizeY);

    mat4 GetViewMat();

    mat4 GetProjectionMat(float displaySizeX, float displaySizeY);

    mat4 GetProjectionMat(float aspectRatio);

    //Set AspectRatio with the SetAspectRatio() method
    mat4 GetProjectionMat();

    //camera
    vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
    vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
    vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

    float znear = 0.1f;
    float zfar = 1000.0f;

    float aspectRatio = 1280 / 720;

private:
    mat4 viewMatrix = mat4();
    mat4 projMatrix = mat4();
};