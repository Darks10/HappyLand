#include "shader_utils.h"

Controller::Controller(){

}

Controller::Controller(Controller &c){

}

Controller::Controller(unsigned int &seed, Bool **update, mat4 *m2, float lambda, float *decaying, float tdecay, Water &w){
	keys['w'] = new Key_w(update, m2);
	keys['s'] = new Key_s(update, m2);
	keys['a'] = new Key_a(update, NULL);
	keys['d'] = new Key_d(update, NULL);
	keys['q'] = new Key_q(update, m2);
	keys['e'] = new Key_e(update, m2);
	keys['x'] = new Key_x();
	keys['z'] = new Key_z(lambda, decaying, tdecay, (Key_x *)keys['x'], seed);
	keys['c'] = new Key_c(seed);
	keys[GLUT_LEFT_BUTTON] = new Mouse_left(update);
	keys['m'] = new Key_m(w.type);
	keys['n'] = new Key_n(w.circ);
	keys['k'] = new Key_k(w.k);
	keys['l'] = new Key_l(w.k);
	keys['o'] = new Key_o(w.q);
	keys['p'] = new Key_p(w.q);
	keys['i'] = new Key_i(w.q);
}

void Controller::KeyPressed(unsigned char key, int x, int y){
	if(keys[key] != 0)
		keys[key]->press();
}

void Controller::KeyReleased(unsigned char key, int x, int y){
	if(keys[key] != 0)
		keys[key]->release();
}


Controller::~Controller(void){
}

void Controller::MousePressed(int button, int state, int x, int y ){
	if(keys[button] != 0){
		if(state == GLUT_DOWN)
			keys[button]->press(x, y);
		else
			keys[button]->release();
	}
}

void Controller::MouseMotion(int x, int y){
	keys[GLUT_LEFT_BUTTON]->motion(x, y);
}
