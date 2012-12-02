#pragma once

using namespace std;

class Key_w;
class Key_s;
class Key_a;
class Key_d;
class Key_q;
class Key_e;
class Key_z;
class Key_x;
class Key_c;
class mouse_Keys;
class Mouse_left;

class Bool{
public:
	Bool(){

	}

	~Bool(){

	}
	virtual Bool* operator!(void){ return this;}
	virtual Bool* operator&&(Bool*){ return this;}

	virtual void move(Mouse_left &w, Camera &c){}
	virtual void move(Key_w &w, Camera &c){}
	virtual void move(Key_s &w, Camera &c){}
	virtual void move(Key_a &w, Camera &c){}
	virtual void move(Key_d &w, Camera &c){}
	virtual void move(Key_q &w, Camera &c){}
	virtual void move(Key_e &w, Camera &c){}
	virtual void move(Key_z &w, float dtime){}
	virtual void move(Key_x &w, Key_z&){}
	virtual void move(Key_c &w){}
	virtual void motion(mouse_Keys &w, int x, int y){}

	virtual void update(mat4 &mvp, mat4 &mvp2,mat4 &mvp3, mat4 &m1, mat4 &m2, mat4 &m3, Camera &c){}
	virtual Bool& getIns(){ return *this; }
};


class True: Bool
{
public:
	static Bool& getInstance()
	{
		static True instance; 

		return instance;
	}
	Bool& getIns(){ return True::getInstance(); }

	Bool* operator!(void);
	Bool* operator&&(Bool*);

	void move(Mouse_left &w, Camera &c);
	void move(Key_w &w, Camera &c);
	void move(Key_s &w, Camera &c);
	void move(Key_a &w, Camera &c);
	void move(Key_d &w, Camera &c);
	void move(Key_q &w, Camera &c);
	void move(Key_e &w, Camera &c);
	void move(Key_z &w, float dtime);
	void move(Key_x &w, Key_z&);
	void move(Key_c &w);
	void motion(mouse_Keys &w, int x, int y);

	void update(mat4 &mvp, mat4 &mvp2,mat4 &mvp3, mat4 &m1, mat4 &m2, mat4 &m3, Camera &c);

private:
	True(){}
	True(True const&){}              // Don't Implement
	void operator=(True const&){} // Don't implement
};

class False: Bool
{
public:
	static Bool& getInstance()
	{
		static False instance; 

		return instance;
	}

	Bool& getIns(){ return False::getInstance(); }

	Bool* operator!(void);
	Bool* operator&&(Bool*);

	void move(Mouse_left &w, Camera &c);
	void move(Key_w &w, Camera &c);
	void move(Key_s &w, Camera &c);
	void move(Key_a &w, Camera &c);
	void move(Key_d &w, Camera &c);
	void move(Key_q &w, Camera &c);
	void move(Key_e &w, Camera &c);
	void move(Key_z &w, float dtime);
	void move(Key_x &w, Key_z&);
	void move(Key_c &w);
	void motion(mouse_Keys &w, int x, int y);

	void update(mat4 &mvp, mat4 &mvp2, mat4 &mvp3, mat4 &m1, mat4 &m2, mat4 &m3, Camera &c){}

private:
	False(){}
	False(False const&){}              // Don't Implement
	void operator=(False const&){} // Don't implement
};


class Keys{
public:
	Bool *isPressed;

	Keys(){ isPressed = &False::getInstance(); }
	~Keys(void){}
	virtual void move(Camera&){}
	virtual void move(int&){}
	virtual void move(float){}
	virtual void move(){}
	virtual void press(){}
	virtual void press(int x, int y){}
	virtual void release(){}
	virtual void changeState(){
		isPressed = !(*isPressed);
	}
	virtual void motion(int x, int y){}
	virtual void print(){ cout << "D:!" << endl;}
};

class mouse_Keys: public Keys{
public:
	Bool **update;
	int x,y;
	int xi, yi;

	mouse_Keys(Bool **update): Keys() { this->update = update; this->x = this->xi = this->y = this->yi = 0; }

	void move(Camera& camera){}
	void press(int x, int y);
	void release();
	void motion(int x, int y);
	void Update(){ *update = &True::getInstance(); }
};

class Mouse_left: public mouse_Keys{
public:
	Mouse_left(Bool **update): mouse_Keys(update) {}

	void move(Camera&);
};

class move_Keys: public Keys{
public:
	Bool **update;
	mat4 *m2;

	move_Keys(Bool **update, mat4 *m2): Keys() { this->update = update; this->m2 = m2; }

	void move(Camera& camera){}
	void press();
	void release();
	void Update(){ *update = &True::getInstance(); }
};

class Key_w: public move_Keys{
public:
	Key_w(Bool **update, mat4 *m2): move_Keys(update, m2) {}

	void move(Camera&);
};

class Key_s: public move_Keys{
public:
	Key_s(Bool **update, mat4 *m2): move_Keys(update, m2) {}

	void move(Camera&);
};


class Key_a: public move_Keys{
public:
	Key_a(Bool **update, mat4 *m2): move_Keys(update, m2) {}

	void move(Camera&);
};

class Key_d: public move_Keys{
public:
	Key_d(Bool **update, mat4 *m2): move_Keys(update, m2) {}

	void move(Camera&);
};

class Key_q: public move_Keys{
public:
	Key_q(Bool **update, mat4 *m2): move_Keys(update, m2) {}

	void move(Camera&);
};

class Key_e: public move_Keys{
public:
	Key_e(Bool **update, mat4 *m2): move_Keys(update, m2) {}

	void move(Camera&);
};

class Key_x: public Keys{
public:
	void press();
	void release();
};

class Key_z: public Keys{
public:
	Bool *first;
	Key_x *x;
	unsigned int *seed;
	float lambda, *decaying, tdecay;
	Key_z(float lambda, float *decaying, float tdecay, Key_x *x, unsigned int &seed) : 
		Keys(), lambda(lambda), decaying(decaying), tdecay(tdecay), x(x), seed(&seed) { first = &False::getInstance(); }

	void move(float dtime);
	void press();
	void release();
};

class Key_c: public Keys{
public:
	unsigned int *seed;
	Key_c(unsigned int &seed){
		this->seed = &seed;
	}

	void move();
	void press();
	void release();
};

