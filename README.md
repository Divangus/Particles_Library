# ParticleLib Documentation

## Table of Contents

1. [Introduction](#introduction)
2. [Video Trailer](#video-trailer)
3. [Utilities](#utilities)
   - [Editable Particles](#editable-particles)
   - [Time-based Scale and Color](#time-based-scale-and-color)
   - [Customizable Emission Rate](#customizable-emission-rate)
   - [Three Types of Billboard](#three-types-of-billboard)
   - [Texture and Animated Texture](#texture-and-animated-texture)
   - [Instanced Rendering](#instanced-rendering)
4. [Download and Implementation Instructions on Visual Studio](#download-and-implementation-instructions-on-visual-studio)
5. [Using the Library](#using-the-library)
   - [ParticleLib Functions](#particlelib-functions)
   - [Particle Properties Struct](#particle-properties-struct)
6. [Third-Party Libraries](#third-party-libraries)
   - [Used in ParticleLib](#used-in-particlelib)
   - [Used in the Demo](#used-in-the-demo)


## Introduction
Welcome to ParticleLib, a C++ static library designed for creating and managing particle systems using OpenGL. This library provides an easy-to-use interface for implementing a variety of particle effects in your projects. Whether you're developing games, simulations, or visual effects, ParticleLib offers the tools you need to bring your particle systems to life.

This library has been developed as a Bachelor´s Degree Final Project.

![Particle Overview](https://github.com/Divangus/Particles_Library/assets/79161120/be614753-3af3-45ce-800d-5d63b56e7f22)

## Video Trailer
[![Snipaste_2024-06-30_17-57-04](https://github.com/Divangus/Particles_Library/assets/79161120/09afd75e-0e41-4465-a6db-09b5abc8f92c)](https://youtu.be/McEw10Ydcrk)
*(Click the image to watch the video trailer)*

## Utilities
### Editable Particles
   - Allows customization of particle attributes such as velocity, color, scale, and lifetime.
     
     <img src="https://github.com/Divangus/Particles_Library/assets/79161120/d6b6f58b-7228-4e30-a06e-76c00178043f" width="640" height="360">

### Time-based Scale and Color
   - Enables scaling and color variation over the lifetime of particles.

     <img src="https://github.com/Divangus/Particles_Library/assets/79161120/a3de66df-15a7-40db-9974-dc579b14c999" width="640" height="360">

### Customizable Emission Rate
   - Control the emission rate to emit particles at a desired rate per second.
     
     <img src="https://github.com/Divangus/Particles_Library/assets/79161120/d3438a03-c191-4a37-8b16-338e8dae7dbf" width="640" height="360">

### Three Types of Billboard
   - Supports three types of billboarding: screen-aligned, world-aligned, and axis-aligned.
     
     <img src="https://github.com/Divangus/Particles_Library/assets/79161120/479875b8-8de2-46c1-b6bb-e6ce1529fe4f" width="640" height="360">

### Texture and Animated Texture
   - Incorporates static textures and supports animated textures for dynamic particle effects.
     
     <img src="https://github.com/Divangus/Particles_Library/assets/79161120/9c299352-cc42-41af-bd2a-70011098ba64" width="640" height="360">

### Instanced Rendering
   - Utilizes instanced rendering for efficient drawing of large numbers of particles.
     
     <img src="https://github.com/Divangus/Particles_Library/assets/79161120/c50acd25-528e-4fd3-a1b1-65ebaf2aa172" width="640" height="360">

## Download and Implementation Instructions on Visual Studio
- Download the Library from the [Release](https://github.com/Divangus/Particles_Library/releases/tag/ParticleLib) of this project and unzip the file. You can also find an executable containing a demo to showcase the capabilities of the library.
  
- Copy the Files of ParticleLib to your project.
  
- Move the `ParticleVShader.glsl` and `ParticleFShader.glsl` to the WorkingDir of the project.
  
- Once in Visual Studio, open the project properties.
  
- Navigate to C/C++ -> General, and add the directory of the include folder and include/ThirdParty to Additional Include Directories.
  
- Go to Linker -> General and add the path where the ParticleLib.Lib file is located to Additional Library Directories, depending on your version of Visual Studio and your Runtime Library (MT/MTd or MD/MDd).
   - To know what Runtime Library are you using in your program go to C/C++ -> Code Generation. 
  
- Go to Linker -> Input and add "ParticleLib.lib" (without the quotes) to Additional Dependencies.
  
- Finally, include "ParticleLib.h" in your code.
  
## Using the Library

### ParticleLib Functions

| Function Name                                                       | Description                                                                                   |
|---------------------------------------------------------------------|-----------------------------------------------------------------------------------------------|
| `bool Init(float screen_width, float screen_height);`                | Initializes the particle system library with the specified screen dimensions.                |
| `void CleanUp();`                                                   | Cleans up and deallocates resources used by the particle system library.                      |
| `void CreateEmitter(std::string name);`                              | Creates an emitter with the given name using default properties.                               |
| `void CreateEmitter(std::string name, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);` | Creates an emitter with specified camera orientation properties.                      |
| `void CreateEmitter(std::string name, ParticleProps particleProperties);` | Creates an emitter with custom particle properties.                                            |
| `void CreateEmitter(std::string name, ParticleProps particleProperties, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);` | Creates an emitter with both custom particle properties and camera orientation.  |
| `void UpdateParticles();`                                           | Emit and updates all particles in the system.                                                           |
| `void UpdateParticles(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);` | Emit and updates particles considering camera orientation.                                         |
| `void OnResize(float new_screen_width, float new_screen_height);`   | Adjusts the particle system rendering to a new screen size.                                   |
| `void RenderParticles();`                                           | Renders all particles in the system.                                                           |
| `void AddTexture(std::string name, std::string path);`              | Add the texture from the provided path to the particle emitter with the specified name.                           |
| `void AddAnimatedTexture(std::string name, std::string path, int atlasColumn, int atlasRows);` | Add the texture atlas from the provided path to the particle emitter with the specified name.           |
| `void RemoveTexture(std::string name);`                              | Removes the texture associated to the particle emitter with the specified name.                           |
| `void SetParticlesPerSecond(std::string name, bool active, const int particlesPerSecond = 1);` | Enables or disables emission rate for the specified emitter.            |
| `void SetBillboard(std::string name, BILLBOARDTYPE typeBB);`         | Sets the billboard type for particles of the specified emitter.                               |
| `void SetAxisBillboard(std::string name, AXISALIGNBB alignAxis);`    | Sets the axis alignment type for particles of the specified emitter.                          |
| `void setMaxParticles(std::string name, int MaxParticles);`          | Sets the maximum number of particles that the emitter can manage.                             |
| `Emitter* GetEmitter(std::string name);`                             | Retrieves a pointer to the emitter object associated with the specified name.                 |


### Particle Properties Struct

| Property/Function Name                                              | Description                                                                                   |
|---------------------------------------------------------------------|-----------------------------------------------------------------------------------------------|
| `glm::vec3 pos`                                                     | Particle initial position.                                                                    |
| `glm::vec3 rot`                                                     | Particle rotation (only works if billboard is set to NO_ALIGN).                               |
| `glm::vec3 speed`                                                   | Particle speed.                                                                               |
| `glm::vec3 speedVariation`                                          | Speed variation for each particle to randomize speed.                                         |
| `glm::vec4 Color`                                                   | Color of the particles. To use color over lifetime, this should be `glm::vec4(0.0f)`.         |
| `glm::vec4 beginColor`                                              | Initial color for color over lifetime. Requires `Color` to be `glm::vec4(0.0f)`.              |
| `glm::vec4 endColor`                                                | Final color for color over lifetime. Requires `Color` to be `glm::vec4(0.0f)`.                |
| `glm::vec3 Scale`                                                   | Scale of the particles. To use scale over lifetime, this should be `glm::vec3(0.0f)`.         |
| `glm::vec3 beginScale`                                              | Initial scale for scale over lifetime. Requires `Scale` to be `glm::vec3(0.0f)`.              |
| `glm::vec3 endScale`                                                | Final scale for scale over lifetime. Requires `Scale` to be `glm::vec3(0.0f)`.                |
| `glm::vec3 scaleVariation`                                          | Scale variation for each particle to randomize size.                                          |
| `bool gravity`                                                      | Whether particles are affected by gravity.                                                    |
| `float LifeTime`                                                    | The lifetime of the particles.                                        |
| `void SetPosition(float x, float y, float z)`                       | Sets the particle's initial position.                                                         |
| `void SetSpeed(float x, float y, float z)`                          | Sets the particle's speed.                                                                    |
| `void SetSpeedVariation(float x, float y, float z)`                 | Sets the speed variation to randomize the speed of the particles.                             |
| `void SetRotation(float x, float y, float z)`                       | Sets the particle's rotation (only works if billboard is set to NO_ALIGN).                    |
| `void SetScale(float x, float y, float z)`                          | Sets the particle's scale. To use scale over lifetime, this should be `(0, 0, 0)`.      |
| `void SetBeginScale(float x, float y, float z)`                     | Sets the initial scale for scale over lifetime. Requires `Scale` to be `(0, 0, 0)`.     |
| `void SetEndScale(float x, float y, float z)`                       | Sets the final scale for scale over lifetime. Requires `Scale` to be `(0, 0, 0)`.       |
| `void SetScaleVariation(float x, float y, float z)`                 | Sets the scale variation to randomize the size of the particles.                              |
| `void SetColor(float r, float g, float b, float a)`                 | Sets the particle's color. To use color over lifetime, this should be `(0, 0, 0)`.      |
| `void SetBeginColor(float r, float g, float b, float a)`            | Sets the initial color for color over lifetime. Requires `Color` to be `(0, 0, 0)`. |
| `void SetEndColor(float r, float g, float b, float a)`              | Sets the final color for color over lifetime. Requires `Color` to be `(0, 0, 0)`. |
| `void SetGravity(bool gravity)`                                     | Sets whether particles are affected by gravity.                                               |
| `void SetLifeTime(bool LifeTime)`                                   | Sets the lifetime of the particles.                                                           |

## Third-Party Libraries

### Used in ParticleLib
- [glad](https://github.com/Dav1dde/glad): Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs.
- [glm](https://github.com/g-truc/glm): OpenGL Mathematics (GLM) is a header-only C++ mathematics library for graphics software.
- [stb](https://github.com/nothings/stb): Single-file public domain libraries for C/C++.

### Used in the Demo
- [MathGeoLib](https://github.com/juj/MathGeoLib): A C++ library for 3D math.
- [glew](https://github.com/nigels-com/glew): The OpenGL Extension Wrangler Library.
- [DevIL](http://openil.sourceforge.net/): Developer's Image Library (DevIL) for image loading.
- [SDL](https://www.libsdl.org/): Simple DirectMedia Layer, a cross-platform development library.
- [ImGUI](https://github.com/ocornut/imgui): Immediate Mode Graphical User Interface for C++.

 ***Happy coding with ParticleLib!***
