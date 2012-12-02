#include "shader_utils.h"

void True::update(mat4 &mvp, mat4 &mvp2,mat4 &mvp3, mat4 &m1, mat4 &m2, mat4 &m3, Camera &c){
	mvp = c.getViewProjection() * m1;
	mvp2 = c.getViewProjection() * m2;
	mvp3 = c.getViewProjection() * m3;
}

Bool* True::operator!(void){
	return &(False::getInstance());
}

Bool* False::operator!(void){
	return &(True::getInstance());
}

Bool* True::operator&&(Bool *o){
	return &(o->getIns());
}

Bool* False::operator&&(Bool *o){
	return &False::getInstance();
}


void move_Keys::press(){
	isPressed = &True::getInstance();
}

void move_Keys::release(){
	isPressed = &False::getInstance();
}

/* Tecla W */

void Key_w::move(Camera &camera){
	isPressed->move(*this, camera);
}

void True::move(Key_w &w, Camera &c){
	c.moveForward();
	*w.m2 = translate(mat4(1.0f), c.getLastDirection()) * (*w.m2);

	w.Update();
}

void False::move(Key_w &w, Camera &c){

}


/* Tecla S */

void Key_s::move(Camera &camera){
	isPressed->move(*this, camera);
}

void True::move(Key_s &w, Camera &c){
	c.moveBackward();
	*w.m2 = translate(mat4(1.0f), c.getLastDirection()) * (*w.m2);

	w.Update();
}

void False::move(Key_s &w, Camera &c){

}

/* Tecla A */

void Key_a::move(Camera &camera){
	isPressed->move(*this, camera);
}

void True::move(Key_a &w, Camera &c){
	c.rotateLeft();

	w.Update();
}

void False::move(Key_a &w, Camera &c){

}

/* Tecla D */

void Key_d::move(Camera &camera){
	isPressed->move(*this, camera);
}

void True::move(Key_d &w, Camera &c){
	c.rotateRight();

	w.Update();
}

void False::move(Key_d &w, Camera &c){

}

/* Tecla Q */

void Key_q::move(Camera &camera){
	isPressed->move(*this, camera);
}

void True::move(Key_q &w, Camera &c){
	c.moveLeft();
	*w.m2 = translate(mat4(1.0f), c.getLastDirection()) * (*w.m2);

	w.Update();
}

void False::move(Key_q &w, Camera &c){

}

/* Tecla E */

void Key_e::move(Camera &camera){
	isPressed->move(*this, camera);
}

void True::move(Key_e &w, Camera &c){
	c.moveRight();
	*w.m2 = translate(mat4(1.0f), c.getLastDirection()) * (*w.m2);

	w.Update();
}

void False::move(Key_e &w, Camera &c){

}

/* Tecla Z */

void Key_z::press(){
	this->changeState();
	*this->decaying = 1.0;
}

void Key_z::release(){

}

void Key_z::move(float dtime){
	isPressed->move(*this, dtime);
}

void True::move(Key_z &w, float dtime){
	float time_mod_tdecay = w.tdecay*(dtime - (int)dtime);
	if (time_mod_tdecay >= 0.0 && time_mod_tdecay < 1.0){
		*w.decaying = exp(-w.lambda*(time_mod_tdecay));
		w.first = &True::getInstance();
	}
	else if(time_mod_tdecay <= 2.0  && time_mod_tdecay >= 1.0){
		*w.decaying = exp(w.lambda*(-1.0 + (time_mod_tdecay - 1.0)));
		(w.x->isPressed)->operator&&(w.first)->move(*w.x, w);
	}
}

void False::move(Key_z &w, float dtime){

}

/* Tecla X */

void Key_x::press(){
	this->changeState();
}

void Key_x::release(){

}


void True::move(Key_x &w, Key_z &z){
	*z.seed = rand();
	z.first = &False::getInstance();
}

void False::move(Key_x &w, Key_z &z){

}

/* Tecla C */

void Key_c::press(){
	*seed = rand();
}

void Key_c::release(){

}

void Key_c::move(){
	isPressed->move(*this);
}

void True::move(Key_c &w){

}

void False::move(Key_c &w){

}
