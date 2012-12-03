#include "shader_utils.h"

/**
 * Display compilation errors from the OpenGL shader compiler
 */
void print_log(GLuint object)
{
  GLint log_length = 0;
  if (glIsShader(object))
	glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
  else if (glIsProgram(object))
	glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
  else {
	fprintf(stderr, "printlog: Not a shader or a program\n");
	return;
  }
 
  char* log = (char*)malloc(log_length);
 
  if (glIsShader(object))
	glGetShaderInfoLog(object, log_length, NULL, log);
  else if (glIsProgram(object))
	glGetProgramInfoLog(object, log_length, NULL, log);
 
  fprintf(stderr, "%s", log);
  free(log);
}

/**
 * Store all the file's contents in memory, useful to pass shaders
 * source code to OpenGL
 */
/* Problem:
 *  We should close the input file before the return NULL; statements but this would lead to a lot of repetition (DRY)
 *   -you could solve this by using goto or by abusing switch/for/while + break or by building an if else mess
 *  better solution: let the user handle the File: char* file_read(const FILE* input)
*/
char* file_read(const char* filename)
{
  FILE* input = fopen(filename, "rb");
  if(input == NULL) return NULL;
 
  if(fseek(input, 0, SEEK_END) == -1) return NULL;
  long size = ftell(input);
  if(size == -1) return NULL;
  if(fseek(input, 0, SEEK_SET) == -1) return NULL;
 
  /*if using c-compiler: dont cast malloc's return value*/
  char *content = (char*) malloc( (size_t) size +1  ); 
  if(content == NULL) return NULL;
 
  fread(content, 1, (size_t)size, input);
  if(ferror(input)) {
	free(content);
	return NULL;
  }
 
  fclose(input);
  content[size] = '\0';
  return content;
}

/**
 * Compile the shader from file 'filename', with error handling
 */
GLuint create_shader(const char* filename, GLenum type)
{
  const GLchar* source = file_read(filename);
  if (source == NULL) {
	fprintf(stderr, "Error opening %s: ", filename); perror("");
	return 0;
  }
  GLuint res = glCreateShader(type);
  const GLchar* sources[2] = {
#ifdef GL_ES_VERSION_2_0
	"#version 100\n"
	"#define GLES2\n",
#else
	"#version 330\n",
#endif
	source };
  glShaderSource(res, 2, sources, NULL);
  free((void*)source);
 
  glCompileShader(res);
  GLint compile_ok = GL_FALSE;
  glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
  if (compile_ok == GL_FALSE) {
	fprintf(stderr, "%s:", filename);
	print_log(res);
	glDeleteShader(res);
	return 0;
  }
 
  return res;
}

void bindUniform(const char *name, GLint& uniform, GLuint& program){
	uniform = glGetUniformLocation(program, name);
	if (uniform == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", name);
		exit(-1);
	}
}

void bindAttribute(const char *name, GLint& attribute, GLuint& program){
	attribute = glGetAttribLocation(program, name);
	if (attribute == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", name);
		exit(-1);
	}
}

void setP1Params(map<string, GLint> &params1){
	params1["attribute_coord3d"] = 0;
	params1["attribute_texcoord"] = 0;
	params1["uniform_mvp"] = 0;
	params1["uniform_mytexture"] = 0;
	params1["uniform_time"] = 0;
	params1["uniform_m_3x3_inv_transp"] = 0;
	params1["uniform_v_inv"] = 0;
	params1["uniform_m"] = 0;
	params1["uniform_nsin"] = 0;
	params1["uniform_type"] = 0;
	params1["uniform_direction"] = 0;
	params1["uniform_mS"] = 0;
	params1["uniform_ratio"] = 0;
	params1["uniform_useed"] = 0;
	params1["uniform_mw"] = 0;
	params1["uniform_vdir"] = 0;
	params1["uniform_k"] = 0;
	params1["uniform_decay"] = 0;
	params1["uniform_circular"] = 0;
}

void setP2Params(map<string, GLint> &params2){
	params2["attribute_vertex"] = 0;
	params2["uniform_mvp"] = 0;
	params2["uniform_cubemap"] = 0;
}

void setP3Params(map<string, GLint> &params3){
	params3["attribute_coord3d"] = 0;
	params3["attribute_colord"] = 0;
	params3["attribute_v_normal"] = 0;
	params3["uniform_mvp"] = 0;
	params3["uniform_m"] = 0;
	params3["uniform_m_3x3_inv_transp"] = 0;
	params3["uniform_v_inv"] = 0;

}

void showFps(int &frame, int &timebase, float &fps){
	frame++;
	float time=glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		fps = frame*1000.0/(time-timebase);
		timebase = time;
		frame = 0;
	}

	stringstream ss(stringstream::in | stringstream::out);
	ss << fps;

	glutSetWindowTitle(ss.str().c_str()); 
}

void printMat4(mat4 m){
	cout << m[0][0] << " " << m[1][0] << " " << m[2][0] << " " << m[3][0] << endl;
	cout << m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << endl;
	cout << m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << endl;
	cout << m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3] << endl << endl;
}

int Mod(int a, int b){
	return ((a%b) + b)%b;
}