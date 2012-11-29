#pragma once

class Terrain: public Object{
public:
GLuint v_grid, v_colors,ctd_tris;
Camera* camera;
unsigned int seed;

Terrain(int, int, int, int, mat4, mat4, Camera*, unsigned int);
Terrain(void);
~Terrain(void);
void render(map<string, GLint>&);
void animate(map<string, GLint>&);
};