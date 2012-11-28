#extension GL_EXT_gpu_shader4: enable

attribute vec3 coord3d;
attribute vec2 texcoord;

varying vec2 f_tex;
varying vec3 coords;
varying vec4 mcoords;

uniform float time;
uniform mat4 mvp;
uniform mat4 m;
uniform int nsin;
uniform int type;
uniform vec2 direction;
uniform float mS;
uniform float ratio;
uniform uint useed;
uniform float mw;
uniform float vdir;
uniform int k;
uniform float decay;
uniform float circular;

const float PI = 3.141529;

uint seed;

float rand( void )
{
	uint a;

	seed *= 16807u;

	a = (seed&0x007fffffu) | 0x40000000u;

	return( (uintBitsToFloat(a) - 2.0f)/2 );
}

vec3 W(){
	float r = rand();
	float L = r*3*mw/2 + mw/2;
	float w = 2*PI/L;
	float A = w*ratio;

	r = rand();
	float S = r*3*mS/2 + mS/2;

	r = rand();
	float rotation = r*2*vdir - vdir;
	vec2 dir = vec2(0.0, 0.0);
	if(circular == 0.0){
		dir = mat2(vec2(cos(rotation), sin(rotation)), vec2(-sin(rotation), cos(rotation))) * direction;
	}
	else{
		if(!all(equal(dir, coord3d.xz))){
			dir = -normalize(coord3d.xz);
		}
	}
	float sen = sin(dot(dir, coord3d.xz)*w + time*S*w);
	switch(type){
		case(0):
			return decay*vec3(0.0, A*sen, 0.0);
		case(1):
			return decay*vec3(0.0, 2*A*pow((sen + 1)/2, k), 0.0);
		case(2):
			float Q = 0.5/(w*A*nsin);
			float cosin = cos(dot(dir, coord3d.xz)*w + time*S*w);
			return decay*vec3(Q*A*dir.x*cosin, A*sen, Q*A*dir.y*cosin);
		default:
			break;
	}
	return vec3(1.0);
}

vec3 sumofW(){
	vec3 sum = vec3(coord3d.x, 0.0, coord3d.z);
	for(int i = 0; i < nsin; i++){
		sum += W();
	}

	return sum;
}

void main(void) {
  seed = useed;
  //y += 0.25*sin(dot(vec2(0.0, 1.0), coord3d.xz)*w2 + time*0.4*w2);

  coords = sumofW();
  mcoords = m * vec4(coords, 1.0);
  gl_Position = mvp * vec4(coords, 1.0);
  //f_color = v_color;
  f_tex = texcoord;
}