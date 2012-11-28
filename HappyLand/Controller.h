#pragma once

using namespace std;

class Controller
{
public:
	map<unsigned char, Keys*> keys;

	Controller(unsigned int&, Bool**, mat4*, float, float*, float);
	Controller();
	Controller(Controller &c);
	void KeyPressed(unsigned char c, int x, int y);
	void KeyReleased(unsigned char c, int x, int y);


	~Controller(void);
};

