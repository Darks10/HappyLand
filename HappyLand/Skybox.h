#pragma once

class Skybox: public Object{
public:
	GLuint text_3d_id, vbo_cube_tex, ibo_cube_tex;

	Skybox(mat4, mat4);
	Skybox(void);
	~Skybox(void);
	void render(map<string, GLint>& params);
	void animate(map<string, GLint> &params);
};

