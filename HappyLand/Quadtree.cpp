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

vector<vector<glm::vec3*>> Quadtree::Mid_point_displacement(int dheight,int level,int r){
    vector<vector<glm::vec3*>> ret;
	vector<glm::vec3*> top;
	vector<glm::vec3*> left;
	vector<glm::vec3*> right;
	vector<glm::vec3*> bot;
	/*Caso base: se llega al ultimo nivel, no seguir iterando*/
    if( level == 0){
		top.push_back(&puntos[0]);
		top.push_back(&puntos[3]);
		left.push_back(&puntos[0]);
		left.push_back(&puntos[1]);
		bot.push_back(&puntos[1]);
		bot.push_back(&puntos[2]);
		right.push_back(&puntos[3]);
		right.push_back(&puntos[2]);
		
    } else{

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
		vector<vector<glm::vec3*>> QuadSI = SI->Mid_point_displacement(newdh,level-1,r);
		vector<vector<glm::vec3*>> QuadSD = SD->Mid_point_displacement(newdh,level-1,r);
		vector<vector<glm::vec3*>> QuadAI = AI->Mid_point_displacement(newdh,level-1,r);
		vector<vector<glm::vec3*>> QuadAD = AD->Mid_point_displacement(newdh,level-1,r);

		/*Actualizar puntos para no generar quiebres*/

		fuse(QuadSI.at(3),QuadSD.at(1));
		fuse(QuadSI.at(2),QuadAI.at(0));
		fuse(QuadAD.at(1),QuadAI.at(3));
		fuse(QuadAD.at(0),QuadSD.at(2));

		top.insert(top.end(),(QuadSI.at(0)).begin(),(QuadSI.at(0)).end());
		top.insert(top.end(),(QuadSD.at(0)).begin(),(QuadSD.at(0)).end());

		left.insert(left.end(),(QuadSI.at(1)).begin(),(QuadSI.at(1)).end());
		left.insert(left.end(),(QuadAI.at(1)).begin(),(QuadAI.at(1)).end());

		bot.insert(bot.end(),(QuadAI.at(2)).begin(),(QuadAI.at(2)).end());
		bot.insert(bot.end(),(QuadAD.at(2)).begin(),(QuadAD.at(2)).end());

		right.insert(right.end(),(QuadSD.at(3)).begin(),(QuadSD.at(3)).end());
		right.insert(right.end(),(QuadAD.at(3)).begin(),(QuadAD.at(3)).end());

	}
	ret.push_back(top);
	ret.push_back(left);
	ret.push_back(bot);
	ret.push_back(right);

	return ret;
        
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