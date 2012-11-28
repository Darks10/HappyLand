#pragma once

class Object{
public:
	mat4 model;
	mat4 mvp;

	Object(void);
	Object(mat4, mat4);
	~Object(void);
	virtual void render(map<string, GLint>&){}
	virtual void animate(map<string, GLint>&){}
};

