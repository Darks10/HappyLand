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