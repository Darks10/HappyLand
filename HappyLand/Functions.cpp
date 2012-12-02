#include "Functions.h"

glm::vec3 displacedpoint(glm::vec3 p1, glm::vec3 p2, int d){
    GLfloat mid = (p1[2]+p2[2])/2;
    int ran = rand();
    GLfloat d_disp = mid + (ran%d - d/2);
    GLfloat x = (p1[0]+ p2[0])/2;
    GLfloat y = (p1[1]+ p2[1])/2;
    GLfloat z = d_disp;
    glm::vec3 ret(x,y,z);
    return ret;
}


glm::vec3 displacedmidpoint(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, int d){
    GLfloat mid = (p1[2]+p2[2]+p3[2]+p4[2])/4;
    int ran = rand();
    GLfloat x = (p1[0]+ p3[0])/2;
    GLfloat y = (p1[1]+ p3[1])/2;
    GLfloat z = mid + (ran%d - d/2);
    glm::vec3 ret(x,y,z);
    return ret;
}

void fuse( vector<glm::vec3*> v1, vector<glm::vec3*> v2){
	/*int i = 0;
	for(glm::vec3 *v : v1){
		*v = *(v2.at(i));
		i++;
	}*/

	for (int i =0;i<v1.size(); i++){
		*v1.at(i) = *v2.at(i);
	}

}