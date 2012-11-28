attribute vec3 vertex;
varying vec3 texCoord;
uniform mat4 mvp;
 
void main() {
    gl_Position = mvp * vec4(vertex, 1.0);
    texCoord = vertex;
}