#include "shader_utils.h"

Skybox::Skybox(void){

}

Skybox::Skybox(mat4 model, mat4 mvp) : Object(model, mvp) {
	GLfloat cube_tex_vertices[] = {
		-1.0,  1.0,  1.0,
		-1.0, -1.0,  1.0,
		1.0, -1.0,  1.0,
		1.0,  1.0,  1.0,
		-1.0,  1.0, -1.0,
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0,  1.0, -1.0,
	};

	glGenBuffers(1, &vbo_cube_tex);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_tex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*24, cube_tex_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLushort cube_indices[] = {
		0, 1, 2, 3,
		3, 2, 6, 7,
		7, 6, 5, 4,
		4, 5, 1, 0,
		0, 3, 7, 4,
		1, 2, 6, 5,
	};

	glGenBuffers(1, &ibo_cube_tex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_tex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*24, cube_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	text_3d_id = SOIL_load_OGL_cubemap
		(
		"xpos.png",
		"xneg.png",
		"ypos.png",
		"yneg.png",
		"zpos.png",
		"zneg.png",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
		);

	if(text_3d_id == 0)
		cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << "skybox" << ")" << endl;

	//GLfloat *pnt = new GLfloat[24];

	//glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_tex);
	//glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*24, pnt);

	//for (int i = 0; i < 24; i++){
	//	cout << pnt[i] << endl;
	//}

	//delete[] pnt;

	/*cout << vbo_cube_tex << endl;*/

}

void Skybox::render(map<string, GLint>& params){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, text_3d_id);
	glUniform1d(params["uniform_cubemap"], /*GL_TEXTURE*/0);

	glEnableVertexAttribArray(params["attribute_vertex"]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_tex);
	glVertexAttribPointer(
		params["attribute_vertex"],   // attribute
		3,                   // number of elements per vertex, here (x,y)
		GL_FLOAT,            // the type of each element
		GL_FALSE,            // take our values as-is
		0, // next coord2d appears every 5 floats
		0                    // offset of the first element
		);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_tex);
	int s;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &s);
	glDrawElements(GL_QUADS, s/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(params["attribute_vertex"]);

	//for(auto& kv : params){
	//	cout << kv.first << " " << kv.second << endl;
	//}

	///*cout << id << endl;*/

	//GLfloat *pnt = new GLfloat[24];

	//glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_tex);
	//glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*24, pnt);

	//for (int i = 0; i < 24; i++){
	//	cout << pnt[i] << endl;
	//}

	//delete[] pnt;

	//GLushort *pnt1 = new GLushort[24];

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_tex);
	//glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLushort)*24, pnt1);

	//for (int i = 0; i < 24; i++){
	//	cout << pnt1[i] << endl;
	//}

	//delete[] pnt1;
}

void Skybox::animate(map<string, GLint> &params){
	glUniformMatrix4fv(params["uniform_mvp"], 1, GL_FALSE, glm::value_ptr(mvp));
}

Skybox::~Skybox(void){
	//glDeleteBuffers(1, &text_3d_id);
	//glDeleteBuffers(1, &vbo_cube_tex);
	//glDeleteBuffers(1, &ibo_cube_tex);
}
