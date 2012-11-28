varying vec3 texCoord;

uniform samplerCube cubemap;
 
void main (void) {
    gl_FragColor = texture(cubemap, texCoord);
}