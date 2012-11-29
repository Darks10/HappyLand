#include "shader_utils.h"


Terrain::Terrain(void){

}

Terrain::Terrain(int n, int m, int r, int level, mat4 model, mat4 mvp, Camera* camera, unsigned int seed)
: Object(model, mvp), camera(camera), seed(seed),  {


	glm::mat4x3 base(
	glm::vec3(-n,m,-50.0),
	glm::vec3(-n,-m,100.0),
	glm::vec3(n,-m,50.0),
	glm::vec3(n,m,-100.0)
	);

	Quadtree * a = new Quadtree(base,0);

	a->Mid_point_formation(6,1.0); 
  
	vector<glm::mat3> triangles=  a->Quad_to_opengl();
  
	GLfloat *triangulos = new GLfloat[triangles.size()*3*3];
	GLfloat *colores = new GLfloat[triangles.size()*3*3];
  
	int mat=0;
	int div=0;
	ctd_tris = triangles.size()*3;
	for(int i=0; i< triangles.size()*3*3;i+=3){
      
		triangulos[i] = triangles.at(mat)[div][0]/1000;
		triangulos[i+1] = triangles.at(mat)[div][2]/1000;
		triangulos[i+2] = triangles.at(mat)[div][1]/1000;
     
		div++;
		if(div == 3){
			mat++;
			div=0;
		}
      
		if (triangulos[i+1]*10< -0.95){
			colores[i] = 0.0;
			colores[i+1] = 0.0;
			colores[i+2] = 0.0;
		} else if (triangulos[i+1]*10< -0.7){
			colores[i] = 0.2;
			colores[i+1] = 0.2;
			colores[i+2] = 0.2;
		} else if (triangulos[i+1]*10< -0.4){
			colores[i] = 0.5;
			colores[i+1] = 0.4;
			colores[i+2] = 0.4;
		} else if (triangulos[i+1]*10< -0.2){
			colores[i] = 0.3;
			colores[i+1] = 0.1;
			colores[i+2] = 0.1;
		} else if (triangulos[i+1]*10< 0.0){
			colores[i] = 0.0;
			colores[i+1] = 0.3;
			colores[i+2] = 0.0;
		} else if (triangulos[i+1]*10< 0.2){
			colores[i] = 0.1;
			colores[i+1] = 0.3;
			colores[i+2] = 0.1;
		} else if ((triangulos[i+1]*10< 0.4)){
			colores[i] = 0.0;
			colores[i+1] = 1.0;
			colores[i+2] = 0.3;
		} else if ((triangulos[i+1]*10< 0.7)){
			colores[i] = 0.2;
			colores[i+1] = 1.0;
			colores[i+2] = 0.7;
		} else if ((triangulos[i+1]*10< 0.95)){
			colores[i] = 0.3;
			colores[i+1] = 1.0;
			colores[i+2] = 0.9;
		} else {
			colores[i] = 0.0;
			colores[i+1] = 1.0;
			colores[i+2] = 1.0;
		}
      
	}


	glGenBuffers(1, &v_grid);
	glBindBuffer(GL_ARRAY_BUFFER, v_grid);
	glBufferData(GL_ARRAY_BUFFER, (triangles.size()*3*3*sizeof(float)), triangulos, GL_STATIC_DRAW);

	glGenBuffers(1, &v_colors);
	glBindBuffer(GL_ARRAY_BUFFER, v_colors);
	glBufferData(GL_ARRAY_BUFFER, (triangles.size()*3*3*sizeof(float)), colores, GL_STATIC_DRAW);
  

	delete[] triangulos;
	delete[] colores;
}

void Terrain::render(map<string, GLint>& sparam){

	glEnableVertexAttribArray(sparam["attribute_coord3d"]);
	glBindBuffer(GL_ARRAY_BUFFER, v_grid);
	glVertexAttribPointer(
		attribute_coord3d, // attribute
		3,                 // number of elements per vertex, here (x,y,z)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);

	glEnableVertexAttribArray(sparam["attribute_texcoord"]);
	glBindBuffer(GL_ARRAY_BUFFER, v_colors);
	glVertexAttribPointer(
		attribute_v_color, // attribute
		3,                 // number of elements per vertex, here (R,G,B)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);

	glDrawArrays(GL_TRIANGLES, 0, ctd_tris);
	glDisableVertexAttribArray(sparam["attribute_coord3d"]);
	glDisableVertexAttribArray(sparam["attribute_texcoord"]);
}

void Terrain::animate(map<string, GLint>& params){
/* ??? */
}


Terrain::~Terrain(void){
// glDeleteBuffers(1, &vbo_cube_vertices);
// glDeleteBuffers(1, &vbo_cube_texcoords);
// glDeleteBuffers(1, &texture_id);
// glDeleteBuffers(1, &ibo_cube_elements);
}