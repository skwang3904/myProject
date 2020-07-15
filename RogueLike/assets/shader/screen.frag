#version 150

#if GL_ES
precision mediump float;
#endif

in vec2 texCoordV;
in vec4 colorV;

out vec4 fragColor;

uniform vec2 resolution;
uniform sampler2D texID;

void main()
{
	//pixel coordinate
	//Bottom Left 0,0 : Top Right resolution.x,resolution.y

	vec4 c = texture(texID, texCoordV) * colorV;
    float grey = 0.299 * c.r + 0.587 * c.g + 0.114 * c.b;

	fragColor = c;
}