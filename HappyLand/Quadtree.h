#ifndef QUADTREE_H
#define	QUADTREE_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Functions.h"
#include "glew.h"
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
using namespace std;

class Quadtree;

class Quadtree {
public:
	Quadtree(glm::mat4x3 puntos, Quadtree * padre );

	void					Mid_point_displacement(int dheight,int level, int r);
    vector<glm::mat3>       Quad_to_opengl();
	void					Clear();
	void					Mid_point_formation(int level,int r);

private:
	int					level;
	glm::mat4x3                             puntos;

	Quadtree *				padre;
	Quadtree *				SI;
	Quadtree *				SD;
	Quadtree *				AI;
	Quadtree *				AD;

};



#endif	/* QUADTREE_H */

