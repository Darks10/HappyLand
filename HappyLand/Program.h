#pragma once
class Program{
public:
	GLuint id;
	GLuint vs;
	GLuint fs;
	map<string, GLint> params;

	Program(map<string, GLint>, string);
	Program(void);
	~Program(void);
};

