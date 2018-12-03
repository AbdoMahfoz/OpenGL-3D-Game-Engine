#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
out vec3 theColor;

uniform vec3 Color;
uniform mat4 MVP;
void main(){

	//gl_Position is one of the few built-in variables : you have to assign some value to it. 
	//Everything else is optional; we�ll see what �everything else� means later.

    gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0);
    theColor = Color;
}

