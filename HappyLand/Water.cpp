#include "shader_utils.h"

Water::Water(void){

}

Water::Water(int n, int m, int rep, mat4 model, mat4 mvp, Camera* camera, unsigned int seed, float decaying) 
	: Object(model, mvp), camera(camera), seed(seed), decaying(decaying) {

	GLfloat *cube_texcoords = new GLfloat[n*m*2*4];
	GLfloat *cube_vertices = new GLfloat[n*m*3*4];

	for(int i = 0; i < n; i++){
		for(int j = 0; j < m; j++){
			cube_vertices[i*m*3 + 3*j] =  1.0*(i - n/2)/10; 
			cube_vertices[i*m*3 + 3*j + 1] =  0.0;
			cube_vertices[i*m*3 + 3*j + 2] = 1.0*(j - m/2)/10;

			cube_vertices[n*m*3 + i*m*3 + 3*j] =  1.0*(i - n/2)/10; 
			cube_vertices[n*m*3 + i*m*3 + 3*j + 1] =  0.0;
			cube_vertices[n*m*3 + i*m*3 + 3*j + 2] = 1.0*(j - m/2)/10;

			cube_vertices[n*m*3*2 + i*m*3 + 3*j] =  1.0*(i - n/2)/10; 
			cube_vertices[n*m*3*2 + i*m*3 + 3*j + 1] =  0.0;
			cube_vertices[n*m*3*2 + i*m*3 + 3*j + 2] = 1.0*(j - m/2)/10;

			cube_vertices[n*m*3*3 + i*m*3 + 3*j] =  1.0*(i - n/2)/10; 
			cube_vertices[n*m*3*3 + i*m*3 + 3*j + 1] =  0.0;
			cube_vertices[n*m*3*3 + i*m*3 + 3*j + 2] = 1.0*(j - m/2)/10;

			cube_texcoords[i*m*2 + 2*j] =  1.0*(i%(n/rep))/(n/rep); 
			cube_texcoords[i*m*2 + 2*j + 1] = 1.0*(j%(m/rep))/(m/rep);

			cube_texcoords[n*m*2 + i*m*2 + 2*j] =  1.0*(i%(n/rep))/(n/rep); 
			cube_texcoords[n*m*2 + i*m*2 + 2*j + 1] = 1.0*(Mod(j-1, m/rep)+1)/(m/rep);

			cube_texcoords[n*m*2*2 + i*m*2 + 2*j] =  1.0*(Mod(i-1, n/rep)+1)/(n/rep); 
			cube_texcoords[n*m*2*2 + i*m*2 + 2*j + 1] = 1.0*(j%(m/rep))/(m/rep);

			cube_texcoords[n*m*2*3 + i*m*2 + 2*j] =  1.0*(Mod(i-1, n/rep)+1)/(n/rep); 
			cube_texcoords[n*m*2*3 + i*m*2 + 2*j + 1] = 1.0*(Mod(j-1, m/rep)+1)/(m/rep);
		}
	}

	glGenBuffers(1, &vbo_cube_texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4*2*n*m, cube_texcoords, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4*3*n*m, cube_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint *cube_elements = new GLuint[2*(n-1)*(m-1)*3];

	for(int i = 1; i < n; i++){
		for(int j = 0; j < m - 1; j++){
			int index = n*m*((j%2) + 2*((i-1)%2));
			cube_elements[2*3*(i-1)*(m-1) + 2*3*j] = index + (i-1)*m + j;
			cube_elements[2*3*(i-1)*(m-1) + 2*3*j + 1] = index + (i-1)*m + j + 1;
			cube_elements[2*3*(i-1)*(m-1) + 2*3*j + 2] = index + i*m + j;
			cube_elements[2*3*(i-1)*(m-1) + 2*3*j + 3] = index + i*m + j;
			cube_elements[2*3*(i-1)*(m-1) + 2*3*j + 4] = index + (i - 1)*m + j + 1;
			cube_elements[2*3*(i-1)*(m-1) + 2*3*j + 5] = index + i*m + j + 1; 
		}
	}

	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*2*3*(n-1)*(m-1), cube_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	texture_id = SOIL_load_OGL_texture
		(
		"agua.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
		);
	if(texture_id == 0)
		cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << "agua.png" << ")" << endl;

	delete[] cube_elements;
	delete[] cube_texcoords;
	delete[] cube_vertices;
}

void Water::render(map<string, GLint>& sparam){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glUniform1i(sparam["uniform_mytexture"], /*GL_TEXTURE*/0);

	glEnableVertexAttribArray(sparam["attribute_coord3d"]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glVertexAttribPointer(
		sparam["attribute_coord3d"],   // attribute
		3,                   // number of elements per vertex, here (x,y)
		GL_FLOAT,            // the type of each element
		GL_FALSE,            // take our values as-is
		3*sizeof(GLfloat), // next coord2d appears every 5 floats
		0                    // offset of the first element
		);

	glEnableVertexAttribArray(sparam["attribute_texcoord"]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
	glVertexAttribPointer(
		sparam["attribute_texcoord"], // attribute
		2,                  // number of elements per vertex, here (x,y)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		2*sizeof(GLfloat),                  // no extra data between each position
		0                   // offset of first element
		);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	int size;  
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size/sizeof(GLuint), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(sparam["attribute_coord3d"]);
	glDisableVertexAttribArray(sparam["attribute_texcoord"]);
}

void Water::animate(map<string, GLint>& params){
	float time =  glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	glUniformMatrix4fv(params["uniform_m"], 1, GL_FALSE, glm::value_ptr(model));
	glUniform1f(params["uniform_time"], time);
	glm::mat4 v_inv = glm::inverse(camera->getView());
	glUniformMatrix4fv(params["uniform_v_inv"], 1, GL_FALSE, glm::value_ptr(v_inv));
	glm::mat3 m_3x3_inv_transp = glm::transpose(glm::inverse(glm::mat3(model)));
	glUniformMatrix3fv(params["uniform_m_3x3_inv_transp"], 1, GL_FALSE, glm::value_ptr(m_3x3_inv_transp));
	glUniform1i(params["uniform_nsin"], 10);
	glUniform1i(params["uniform_type"], 2);
	glm::vec2 direction = glm::vec2(0.0, 1.0);
	glUniform2fv(params["uniform_direction"], 1, glm::value_ptr(direction));
	glUniform1f(params["uniform_mS"], 0.6);
	glUniform1f(params["uniform_ratio"], 0.015);
	glUniform1ui(params["uniform_useed"], seed);
	glUniform1f(params["uniform_mw"], 2.0);
	glUniform1f(params["uniform_vdir"], 3.141592/4);
	glUniform1i(params["uniform_k"], 3);

	glUniform1f(params["uniform_decay"], decaying);
	glUniform1f(params["uniform_circular"], 0.0);
	glUniformMatrix4fv(params["uniform_mvp"], 1, GL_FALSE, glm::value_ptr(mvp));
}


Water::~Water(void){
// 	glDeleteBuffers(1, &vbo_cube_vertices);
// 	glDeleteBuffers(1, &vbo_cube_texcoords);
// 	glDeleteBuffers(1, &texture_id);
// 	glDeleteBuffers(1, &ibo_cube_elements);
}
