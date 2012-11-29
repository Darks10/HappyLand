#pragma once
#ifndef _CREATE_SHADER_H
#define _CREATE_SHADER_H
/* Using the standard output for fprintf */
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <math.h>
#include "glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glut.h>
#include <iostream>
#include <SOIL.h>
#include <map>
#include "Camera.h"
#include "Keys.h"
#include "Controller.h"
#include "Object.h"
#include "Water.h"
#include "Skybox.h"
#include "Program.h"
#include "Terrain.h"
#include "Quadtree.h"
#include "Functions.h"
#include <string>
#include <sstream>

#define GLEW_STATIC
char* file_read(const char* filename);
void print_log(GLuint object);
GLuint create_shader(const char* filename, GLenum type);
int Mod(int, int);
void bindUniform(const char *name, GLint& uniform, GLuint& program);
void bindAttribute(const char *name, GLint& attribute, GLuint& program);
void setP1Params(map<string, GLint> &params1);
void setP2Params(map<string, GLint> &params2);
void setP3Params(map<string, GLint> &params3);
void showFps(int &frame, int &timebase, float &);
void printMat4(mat4 m);
#endif