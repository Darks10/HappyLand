#ifndef FUNCIONES_H
#define	FUNCIONES_H

#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "glew.h"
#include <GL/glut.h>
#include <vector>
using namespace std;

glm::vec3 displacedpoint(glm::vec3 p1, glm::vec3 p2, int d);
glm::vec3 displacedmidpoint(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,glm::vec3 p4, int d);
void fuse(vector<glm::vec3*> v1, vector<glm::vec3*> v2);

#endif	/* FUNCIONES_H */

