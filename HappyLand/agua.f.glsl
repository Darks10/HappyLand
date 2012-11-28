#extension GL_EXT_gpu_shader4: enable

varying vec3 coords;
varying vec4 mcoords;
varying vec2 f_tex;

uniform sampler2D mytexture;
uniform mat3 m_3x3_inv_transp;
uniform mat4 v_inv;
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

vec3 dW(){
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
	if(!all(equal(dir, coords.xz))){
		dir = -normalize(coords.xz);
	}
  }
  float param = dot(dir, coords.xz)*w + time*S*w;
	switch(type){
		case(0):
			return decay*vec3(-w*dir.x*A*cos(param), 0.0, -w*dir.y*A*cos(param));
		case(1):
			return decay*vec3(-k*dir.x*w*A*cos(param)*pow((sin(param) + 1)/2, k), 0.0,-k*dir.y*w*A*cos(param)*pow((sin(param) + 1)/2, k));
		case(2):
			float Q = 0.5/(w*A*nsin);
			return decay*vec3(-w*dir.x*A*cos(param), -Q*w*A*sin(param), -w*dir.y*A*cos(param));
		default:
			break;
	}
	return vec3(1.0);
}

vec3 sumofdW(){
	vec3 sum = vec3(0.0, 1.0, 0.0);
	for(int i = 0; i < nsin; i++){
		sum += dW();
	}

	return sum;
}

struct lightSource
{
  vec4 position;
  vec4 diffuse;
  vec4 specular;
  float constantAttenuation, linearAttenuation, quadraticAttenuation;
  float spotCutoff, spotExponent;
  vec3 spotDirection;
};
lightSource light0 = lightSource(
  vec4(0.0,  2.0,  0.0, 0.0),
  vec4(1.0,  1.0,  1.0, 1.0),
  vec4(1.0,  1.0,  1.0, 1.0),
  0.0, 0.4, 0.0,
  180.0, 0.0,
  vec3(0.0, 0.0, 0.0)
);
vec4 scene_ambient = vec4(0.0, 0.0, 0.0, 1.0);
 
struct material
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

//varying vec3 f_color;
void main(void) {
  seed = useed;
  vec3 v_normal = sumofdW();
  vec3 normalDirection = normalize(m_3x3_inv_transp * v_normal);
  vec3 viewDirection = normalize(vec3(v_inv * vec4(0.0, 0.0, 0.0, 1.0) - mcoords));
  vec3 lightDirection;
  float attenuation;

  material mymaterial = material(
	  texture2D(mytexture, f_tex),
	  texture2D(mytexture, f_tex),
	  vec4(1.0, 1.0, 1.0, 1.0),
	  300.0
  );

  if (light0.position.w == 0.0) // directional light
	{
	  attenuation = 1.0; // no attenuation
	  lightDirection = normalize(vec3(light0.position));
	}
  else // point or spot light (or other kind of light)
	{
	  vec3 vertexToLightSource = vec3(light0.position - mcoords);
	  float distance = length(vertexToLightSource);
	  lightDirection = normalize(vertexToLightSource);
	  attenuation = 1.0 / (light0.constantAttenuation
						   + light0.linearAttenuation * distance
						   + light0.quadraticAttenuation * distance * distance);
 
	  if (light0.spotCutoff <= 90.0) // spotlight
		{
		  float clampedCosine = max(0.0, dot(-lightDirection, normalize(light0.spotDirection)));
		  if (clampedCosine < cos(radians(light0.spotCutoff))) // outside of spotlight cone
			{
			  attenuation = 0.0;
			}
		  else
			{
			  attenuation = attenuation * pow(clampedCosine, light0.spotExponent);
			}
		}
	}
 
  vec3 ambientLighting = vec3(scene_ambient) * vec3(mymaterial.ambient);
 
  vec3 diffuseReflection = attenuation
	* vec3(light0.diffuse) * vec3(mymaterial.diffuse)
	* max(0.0, dot(normalDirection, lightDirection));
 
  vec3 specularReflection;
  if (dot(normalDirection, lightDirection) < 0.0) // light source on the wrong side?
	{
	  specularReflection = vec3(0.0, 0.0, 0.0); // no specular reflection
	}
  else // light source on the right side
	{
	  specularReflection = attenuation * vec3(light0.specular) * vec3(mymaterial.specular)
		* pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)),
			  mymaterial.shininess);
	}
 
  vec4 color = vec4(ambientLighting + diffuseReflection + specularReflection, 1.0);
  gl_FragColor = color;
}