#version 150

#if GL_ES
precision mediump float;
#endif

out vec4 fragColor;

in vec2 texCoordV;
in vec4 colorV;

uniform sampler2D texBase;
uniform sampler2D texBlend;

void main()
{
	vec4 c = texture(texBlend, texCoordV);
    vec3 grey = vec3(0.299 * c.r + 0.587 * c.g + 0.114 * c.b);
	fragColor = vec4(grey, c.a * colorV.a);
	//fragColor = vec4(1.0f);
}
