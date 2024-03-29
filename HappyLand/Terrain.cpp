#include "shader_utils.h"
#include <limits>

typedef std::numeric_limits< double > dbl;

Terrain::Terrain(void){

}

Terrain::Terrain(int n, int m, int level, float r, mat4 model, mat4 mvp, Camera* camera, unsigned int seed)
: Object(model, mvp), camera(camera), seed(seed) {


	glm::mat4x3 base(
		glm::vec3(-n,m,-50.0),
		glm::vec3(-n,-m,100.0),
		glm::vec3(n,-m,50.0),
		glm::vec3(n,m,-100.0)
	);
	map<string,glm::vec3> map1;
	map<string,int> map2;
	Quadtree * a = new Quadtree(base,0);

	a->Mid_point_formation(level,r); 
  
	vector<glm::mat3> triangles=  a->Quad_to_opengl();
  
	GLfloat *triangulos = new GLfloat[triangles.size()*3*3];
	GLfloat *normales = new GLfloat[triangles.size()*3*3];
	GLfloat *colores = new GLfloat[triangles.size()*3*3];
  
	int mat=0;
	int div=0;
	ctd_tris = triangles.size()*3;
	for(int i=0; i< triangles.size()*3*3;i+=3){
	  
		triangulos[i] = triangles.at(mat)[div][1]/1000;
		triangulos[i+1] = triangles.at(mat)[div][2]/1000;
		triangulos[i+2] = triangles.at(mat)[div][0]/1000;
	 
		div++;
		if(div == 3){
			glm::vec3 v1(
				(triangles.at(mat)[1][0]/1000 - triangles.at(mat)[0][0]/1000),
				(triangles.at(mat)[1][1]/1000 - triangles.at(mat)[0][1]/1000),
				(triangles.at(mat)[1][2]/1000 - triangles.at(mat)[0][2]/1000));
			glm::vec3 v2(
				(triangles.at(mat)[2][0]/1000 - triangles.at(mat)[0][0]/1000),
				(triangles.at(mat)[2][1]/1000 - triangles.at(mat)[0][1]/1000),
				(triangles.at(mat)[2][2]/1000 - triangles.at(mat)[0][2]/1000));
			glm::vec3 cross(
				(v1[1]*v2[2] - v1[2]*v2[1]),
				(v1[2]*v2[0] - v1[0]*v2[2]),
				(v1[0]*v2[1] - v1[1]*v2[0]));
			for(int k = 0; k<3;k++){
				stringstream buffer;
				buffer.precision(dbl::digits10);
				buffer <<triangles.at(mat)[k][0]<< " " <<triangles.at(mat)[k][1]<< " " <<triangles.at(mat)[k][2];
				map1[buffer.str()]+= cross;
				map2[buffer.str()]++;
			}
			mat++;
			div=0;

		}
	  
		if (triangulos[i+1]*10< -1.0){
			colores[i] = 0.1;
			colores[i+1] = 0.1;
			colores[i+2] = 0.1;
		} else if (triangulos[i+1]*10< -0.5){
			colores[i] = 0.2;
			colores[i+1] = 0.2;
			colores[i+2] = 0.2;
		} else if (triangulos[i+1]*10< 0.0){
			colores[i] = 0.5;
			colores[i+1] = 0.4;
			colores[i+2] = 0.4;
		} else if (triangulos[i+1]*10< 0.5){
			colores[i] = 0.3;
			colores[i+1] = 0.1;
			colores[i+2] = 0.1;
		} else if (triangulos[i+1]*10< 1.0){
			colores[i] = 0.0;
			colores[i+1] = 0.3;
			colores[i+2] = 0.0;
		} else if (triangulos[i+1]*10< 2.0){
			colores[i] = 0.1;
			colores[i+1] = 0.3;
			colores[i+2] = 0.1;
		} else if ((triangulos[i+1]*10< 3.0)){
			colores[i] = 0.0;
			colores[i+1] = 1.0;
			colores[i+2] = 0.3;
		} else if ((triangulos[i+1]*10< 3.5)){
			colores[i] = 0.2;
			colores[i+1] = 1.0;
			colores[i+2] = 0.7;
		} else if ((triangulos[i+1]*10< 4.0)){
			colores[i] = 0.3;
			colores[i+1] = 1.0;
			colores[i+2] = 0.9;
		} else {
			colores[i] = 0.0;
			colores[i+1] = 1.0;
			colores[i+2] = 1.0;
		}
	  
	}
	mat = 0;
	div = 0;
	for(int i=0; i< triangles.size()*3*3;i+=3){

		stringstream buffer;
		buffer.precision(dbl::digits10);
		buffer <<triangles.at(mat)[div][0]<< " " << triangles.at(mat)[div][1]<< " " <<triangles.at(mat)[div][2];
		glm::vec3 v = map1[buffer.str()];
		int val = map2[buffer.str()];

		normales[i] = v[1]/val;
		normales[i+1] = v[2]/val;
		normales[i+2] = v[0]/val;

		div++;
		if(div == 3){
			mat++;
			div=0;
		}
	}

	glGenBuffers(1, &v_grid);
	glBindBuffer(GL_ARRAY_BUFFER, v_grid);
	glBufferData(GL_ARRAY_BUFFER, (triangles.size()*3*3*sizeof(float)), triangulos, GL_STATIC_DRAW);

	glGenBuffers(1, &v_colors);
	glBindBuffer(GL_ARRAY_BUFFER, v_colors);
	glBufferData(GL_ARRAY_BUFFER, (triangles.size()*3*3*sizeof(float)), colores, GL_STATIC_DRAW);

	glGenBuffers(1, &v_normals);
	glBindBuffer(GL_ARRAY_BUFFER, v_normals);
	glBufferData(GL_ARRAY_BUFFER, (triangles.size()*3*3*sizeof(float)), normales, GL_STATIC_DRAW);
  

	delete[] triangulos;
	delete[] colores;
	delete[] normales;
}

void Terrain::render(map<string, GLint>& sparam){

	glEnableVertexAttribArray(sparam["attribute_coord3d"]);
	glBindBuffer(GL_ARRAY_BUFFER, v_grid);
	glVertexAttribPointer(
		sparam["attribute_coord3d"], // attribute
		3,                 // number of elements per vertex, here (x,y,z)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);

	glEnableVertexAttribArray(sparam["attribute_colord"]);
	glBindBuffer(GL_ARRAY_BUFFER, v_colors);
	glVertexAttribPointer(
		sparam["attribute_colord"], // attribute
		3,                 // number of elements per vertex, here (R,G,B)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);

	glEnableVertexAttribArray(sparam["attribute_v_normal"]);
	glBindBuffer(GL_ARRAY_BUFFER, v_normals);
	glVertexAttribPointer(
		sparam["attribute_v_normal"], // attribute
		3,                 // number of elements per vertex, here (x,y,z)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);

	glDrawArrays(GL_TRIANGLES, 0, ctd_tris);
	glDisableVertexAttribArray(sparam["attribute_coord3d"]);
	glDisableVertexAttribArray(sparam["attribute_colord"]);
	glDisableVertexAttribArray(sparam["attribute_v_normals"]);
}

void Terrain::animate(map<string, GLint>& params){
	glUniformMatrix4fv(params["uniform_m"], 1, GL_FALSE, glm::value_ptr(model));
	glm::mat4 v_inv = glm::inverse(camera->getView());
	glUniformMatrix4fv(params["uniform_v_inv"], 1, GL_FALSE, glm::value_ptr(v_inv));
	glm::mat3 m_3x3_inv_transp = glm::transpose(glm::inverse(glm::mat3(model)));
	glUniformMatrix3fv(params["uniform_m_3x3_inv_transp"], 1, GL_FALSE, glm::value_ptr(m_3x3_inv_transp));
	glUniformMatrix4fv(params["uniform_mvp"], 1, GL_FALSE, glm::value_ptr(mvp));
}


Terrain::~Terrain(void){
// glDeleteBuffers(1, &vbo_cube_vertices);
// glDeleteBuffers(1, &vbo_cube_texcoords);
// glDeleteBuffers(1, &texture_id);
// glDeleteBuffers(1, &ibo_cube_elements);
}