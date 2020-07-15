#version 150

#if GL_ES
precision mediump float;
#endif

in vec2 texCoordV;
in vec4 colorV;

out vec4 fragColor;

uniform vec2 resolution;
uniform sampler2D texBase;
uniform sampler2D texBlend;



void main()
{
	vec4 c = texture(texBlend, texCoordV) * colorV;
    float grey = 0.299 * c.r+ 0.587 * c.g + 0.114 * c.b;


	fragColor = vec4(1);
	

}