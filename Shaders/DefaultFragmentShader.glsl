#version 330 core


// Ouput data
in vec3 theColor;
out vec3 color;

void main()
{

	// we set the color of each fragment to red.
	color = theColor;

}