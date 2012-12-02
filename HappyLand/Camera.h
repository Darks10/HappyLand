#pragma once

#define delta 0.15

using namespace glm;

class Camera{
public:
	vec3 eye;
	vec3 center;
	vec3 up;
	vec3 direction;
	mat4 view;
	mat4 projection;
	mat4 vp;

	static const int a = 3;

	Camera(void);
	Camera(vec3 eye, vec3 center, vec3 up, int width, int height);
	~Camera(void);

	void moveRight();
	void moveLeft();
	void moveForward();
	void moveBackward();
	void rotateRight();
	void rotateLeft();
	void rotateVertical(vec2 p1, vec2 p2);

	mat4 getViewProjection();
	vec3 eyeToCenter();
	vec3 getLastDirection();
	mat4 getView();

private:
	void moveHorizontal(float d);
	void rotate(float d);
	void moveVertical(float d);
	void setView();
};

