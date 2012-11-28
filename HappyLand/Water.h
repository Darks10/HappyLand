#pragma once

class Water: public Object{
public:
	GLuint vbo_cube_vertices, vbo_cube_texcoords, texture_id;
	GLuint ibo_cube_elements;
	Camera* camera;
	unsigned int seed;
	float decaying;

	Water(int, int, int, mat4, mat4, Camera*, unsigned int, float);
	Water(void);
	~Water(void);
	void render(map<string, GLint>&);
	void animate(map<string, GLint>&);
};

