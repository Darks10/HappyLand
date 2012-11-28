#include "shader_utils.h"

Camera::Camera(void){
	this->eye = vec3(1.0, 1.0, 1.0);
	this->center = vec3(0.0, 0.0, 1.0);
	this->up = vec3(0.0, 1.0, 0.0);
	projection = perspective(45.0f, 1.0f*800/600, 0.1f, 20.0f);
	this->setView();
}

Camera::Camera(vec3 eye, vec3 center, vec3 up, int width, int height){
	this->eye = eye;
	this->center = center;
	this->up = up;
	projection = perspective(45.0f, 1.0f*width/height, 0.1f, 20.0f);
	this->setView();
}

void Camera::moveLeft(){
	this->moveHorizontal(-delta);
}

void Camera::moveRight(){
	this->moveHorizontal(delta);
}

void Camera::rotateRight(){
	this->rotate(-a*delta);
}

void Camera::rotateLeft(){
	this->rotate(a*delta);
}

void Camera::moveBackward(){
	this->moveVertical(-delta);
}

void Camera::moveForward(){
	this->moveVertical(delta);
}

vec3 Camera::eyeToCenter(){
	return center - eye;
}

void Camera::moveHorizontal(float d){
	direction = this->eyeToCenter();
	direction = direction - glm::dot(direction, up)*up;
	direction = normalize(cross(direction, up)) * d;
	center += direction;
	eye += direction;

	this->setView();
}

void Camera::rotate(float d){
	center = glm::rotate(center - eye, d, up) + eye;

	this->setView();
}

void Camera::moveVertical(float d){
	direction = center - eye;
	direction = direction - glm::dot(direction, up)*up;
	direction = glm::normalize(direction) * d;
	center += direction;
	eye += direction;

	this->setView();
}

void Camera::setView(){
	view = lookAt(eye, center, up);
	vp = projection * view;
}

mat4 Camera::getView(){
	return view;
}

mat4 Camera::getViewProjection(){
	return vp;
}

vec3 Camera::getLastDirection(){
	return this->direction;
}

Camera::~Camera(void){
}

