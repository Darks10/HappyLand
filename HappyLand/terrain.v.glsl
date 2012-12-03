attribute vec3 coord3d;
attribute vec3 colord;
attribute vec3 v_normal;

varying vec3 f_color;

uniform mat4 mvp;
uniform mat3 m_3x3_inv_transp;
uniform mat4 m;
uniform mat4 v_inv;

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

void main(void) {
  vec4 mcoords = m * vec4(coord3d, 1.0); 
  vec3 normalDirection = normalize(m_3x3_inv_transp * v_normal);
  vec3 viewDirection = normalize(vec3(v_inv * vec4(0.0, 0.0, 0.0, 1.0) - mcoords));
  vec3 lightDirection;
  float attenuation;

  material mymaterial = material(
	  vec4(colord, 1.0),
	  vec4(colord, 1.0),
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
 
  vec3 color = ambientLighting + diffuseReflection + specularReflection;

  gl_Position = mvp * vec4(coord3d, 1.0);
  f_color = color;
}