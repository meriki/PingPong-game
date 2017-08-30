#version 330

layout (location = 0)in vec3 pos;

uniform mat4 projectioonMatrix;
uniform vec3 delta;

void main() {

	gl_Position = vec4(pos, 1.0f);


}
