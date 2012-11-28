#include "shader_utils.h"

Program::Program(void){

}

Program::Program(map<string, GLint> params, string shader_name) : params(params) {
	if ((vs = create_shader((shader_name+".v.glsl").c_str(), GL_VERTEX_SHADER))   == 0) exit(-1);
	if ((fs = create_shader((shader_name+".f.glsl").c_str(), GL_FRAGMENT_SHADER)) == 0) exit(-1);

	GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

	id = glCreateProgram();
	glAttachShader(id, vs);
	glAttachShader(id, fs);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		fprintf(stderr, "glLinkProgram:");
		print_log(id);
		exit(-1);
	}

	for(auto& kv : this->params){
		if(kv.first.substr(0, 7) == "uniform"){
			bindUniform(kv.first.substr(8).c_str(), kv.second, id); /*uniform*/
		}
		else{
			bindAttribute(kv.first.substr(10).c_str(), kv.second, id); /*attribute*/
		}
	}
}


Program::~Program(void){
	//glDeleteProgram(id);
}
