
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include "shader_utils.h"
/* Using the GLUT library for the base windowing setup */


using namespace std;
using namespace glm;

int screen_width=800, screen_height=600;
float tdecay = 2.0;
float lambda = 1.0;

Camera camera;
Controller controller;
Water water;
Skybox skybox;
Program p1;
Program p2;
Bool *update = &False::getInstance();

void onDisplay()
{
  /* Clear the background as white */
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glUseProgram(p1.id);
  water.render(p1.params);

  glUseProgram(p2.id);
  skybox.render(p2.params);

  glutSwapBuffers();
}

/*
Function: init_resources
Receives: void
Returns: int
This function creates all GLSL related stuff
explained in this example.
Returns 1 when all is ok, 0 with a displayed error
*/

unsigned int mirand = 1;

float sfrand( void )
{
	unsigned int a;

	mirand *= 16807;

	a = (mirand&0x007fffff) | 0x40000000;

	return( (*((float*)&a) - 2.0f)/2 );
}


int init_resources(void)
{
  srand(time(NULL));
  rand();
  unsigned int seed = rand();

  mirand = seed;
  float r = sfrand();
  float L = r*3*2.0/2 + 2.0/2;
  float w = 2*3.141592/L;
  float A = w*0.05;

  float r1 = sfrand();
  float S = r1*3*0.4/2 + 0.4/2;

  printf("r=%f, L=%f, w=%f, A=%f, r=%f, S=%f\n", r, L, w, A, r1, S);

  float decaying = 1.0;

  mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
  mat4 model2 = scale(mat4(1.0), vec3(15.0, 15.0, 15.0));

  camera = Camera(vec3(0.0, 2.0, 0.0), vec3(0.0, 0.0, -4.0), vec3(0.0, 1.0, 0.0), screen_width, screen_height);

  mat4 mvp = camera.getViewProjection() * model;
  mat4 mvp2 = camera.getViewProjection() * model2;

  water = Water(400, 400, 10, model, mvp, &camera, seed, decaying);
  skybox = Skybox(model2, mvp2);

  controller = Controller(water.seed, &update, &skybox.model, lambda, &water.decaying, tdecay);

  map<string, GLint> params1;
  map<string, GLint> params2;
  setP1Params(params1);
  setP2Params(params2);

  p1 = Program(params1, "../HappyLand/agua");
  p2 = Program(params2, "../HappyLand/skybox");

  return 1;
}
int frame = 0, timebase = 0;
float fps;

void idle()
{
	float time =  glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	float dtime = time/tdecay;
	controller.keys['z']->move(dtime);

	controller.keys['w']->move(camera);
	controller.keys['s']->move(camera);
	controller.keys['a']->move(camera);
	controller.keys['d']->move(camera);
	controller.keys['q']->move(camera);
	controller.keys['e']->move(camera);

	update->update(water.mvp, skybox.mvp, water.model, skybox.model, camera);

	update = &False::getInstance();

	glUseProgram(p1.id);
	water.animate(p1.params);

	glUseProgram(p2.id);
	skybox.animate(p2.params);

	glutPostRedisplay();

	showFps(frame, timebase, fps);
}

void onReshape(int width, int height) {
  screen_width = 800;
  screen_height = 600;
  glViewport(0, 0, screen_width, screen_height);
}

void keyboardup(unsigned char key, int x, int y){
	controller.KeyReleased(key, x, y);
}

void keyboard(unsigned char key, int x, int y){
	controller.KeyPressed(key, x, y);
}

int main(int argc, char* argv[])
{
  /* Glut-related initialising functions */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(screen_width, screen_height);
  glutCreateWindow("My First Water Model");
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 
  /* Extension wrangler initialising */
  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK)
  {
	fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
	return EXIT_FAILURE;
  }
 
  /* When all init functions runs without errors,
  the program can initialise the resources */
  if (1 == init_resources())
  {
	/* We can display it if everything goes OK */
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardup);
	glutReshapeFunc(onReshape);
	glutDisplayFunc(onDisplay);
	glutIdleFunc(idle);
	glutMainLoop();
  }

  return EXIT_SUCCESS;
}