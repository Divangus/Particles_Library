#ifndef PARTICLE_GLOBALS
#define PARTICLE_GLOBALS

#if defined __glew_h__
#include "glew.h"
#else
#include <glad/glad.h>
#endif // !__glad_h_


#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/quaternion.hpp>
#include <glm/glm/gtx/quaternion.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>  


#pragma warning(disable : 4267) // conversion from X to Y, possible loss of data


#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

#endif // !PARTICLE_GLOBALS

