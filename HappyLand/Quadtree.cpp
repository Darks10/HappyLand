#include "Quadtree.h"

Quadtree::Quadtree(glm::mat4x3 _puntos, Quadtree * _padre ) :
	puntos                  ( _puntos ),
	padre                   ( _padre ),
        level                   (0)
        /*SI                      ( 0 ),
        SD                      ( 0 ),
        AI                      ( 0 ),
        AD                      ( 0 )*/
{
}

void Quadtree::Mid_point_formation(int level,int r){
	int dheight = ceil((abs(puntos[0][0]-puntos[1][0]) + abs(puntos[0][1]-puntos[1][1])) / 2);
	srand(time(NULL)); 
    Mid_point_displacement(dheight,level,r);
}

void Quadtree::Mid_point_displacement(int dheight,int level,int r){
    /*Caso base: se llega al ultimo nivel, no seguir iterando*/
    if( level == 0){
        return;
    }
    /*Obtencion de nuevos puntos*/
    glm::vec3 sup = displacedpoint(puntos[0],puntos[3],dheight);    
    glm::vec3 izq = displacedpoint(puntos[0],puntos[1],dheight);    
    glm::vec3 abj = displacedpoint(puntos[1],puntos[2],dheight);
    glm::vec3 der = displacedpoint(puntos[2],puntos[3],dheight);    
    glm::vec3 mid = displacedmidpoint(sup,izq,abj,der,dheight);
    
    glm::mat4x3 msi(puntos[0],izq,mid,sup);    
    glm::mat4x3 msd(sup,mid,der,puntos[3]);    
    glm::mat4x3 mai(izq,puntos[1],abj,mid);    
    glm::mat4x3 mad(mid,abj,puntos[2],der);
    
    /*Agregar quadtrees hijos y actualizar lvl*/
    SI = new Quadtree(msi,this);
    SD = new Quadtree(msd,this);
    AI = new Quadtree(mai,this);
    AD = new Quadtree(mad,this);
    
    this->level=level;
    int newdh = ceil(dheight/pow(2.0,r));
    /* Seguir el fractal*/
    SI->Mid_point_displacement(newdh,level-1,r);
    SD->Mid_point_displacement(newdh,level-1,r);
    AI->Mid_point_displacement(newdh,level-1,r);
    AD->Mid_point_displacement(newdh,level-1,r);
        
}

vector<glm::mat3>   Quadtree::Quad_to_opengl(){
    
    if (level == 0){
        vector<glm::mat3> base;
        glm::mat3 t1(puntos[0],puntos[1],puntos[2]);
        glm::mat3 t2(puntos[0], puntos[2],puntos[3]);
        base.push_back(t1);
        base.push_back(t2);
        return base;
    }else{
        vector<glm::mat3> ret,matsi,matsd,matai,matad;
        matsi = SI->Quad_to_opengl();
        matsd = SD->Quad_to_opengl();
        matai = AI->Quad_to_opengl();
        matad = AD->Quad_to_opengl();
        ret.reserve(matsi.size()+matsd.size()+matai.size()+matad.size());
        ret.insert(ret.end(),matsi.begin(),matsi.end());
        ret.insert(ret.end(),matsd.begin(),matsd.end());
        ret.insert(ret.end(),matai.begin(),matai.end());
        ret.insert(ret.end(),matad.begin(),matad.end());
        
        return ret;
    }
    
}