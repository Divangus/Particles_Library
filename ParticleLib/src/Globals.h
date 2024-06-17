#ifndef GLOBALS
#define GLOBALS

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>  


#pragma warning(disable : 4267) // conversion from X to Y, possible loss of data

typedef char                   i8;
typedef short                  i16;
typedef int                    i32;
typedef long long int          i64;
typedef unsigned char          u8;
typedef unsigned short         u16;
typedef unsigned int           u32;
typedef unsigned long long int u64;
typedef float                  f32;
typedef double                 f64;

typedef glm::vec2  vec2;
typedef glm::vec3  vec3;
typedef glm::vec4  vec4;
typedef glm::mat3  mat3;
typedef glm::mat4  mat4;
typedef glm::quat  quat;
typedef glm::ivec2 ivec2;
typedef glm::ivec3 ivec3;
typedef glm::ivec4 ivec4;

struct Image
{
    void* pixels;
    ivec2 size;
    i32   nchannels;
    i32   stride;
};

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

vec3 lerp(const vec3 a, const vec3 b, float t) {
    return a + t * (b - a);
}

vec4 lerp(const vec4 a, const vec4 b, float t) {
    return a + t * (b - a);
}

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

#endif // !GLOBALS

