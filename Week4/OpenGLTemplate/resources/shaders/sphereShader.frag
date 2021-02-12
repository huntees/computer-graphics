#version 400 core

in vec3 vColour;			// Note: colour is smoothly interpolated (default)
out vec4 vOutputColour;

in float fIntensity;

uniform int levels;

void main()
{	
	vec3 quantisedColour = floor(vColour * levels) / levels;

	vOutputColour = vec4(quantisedColour, 1.0);
}
