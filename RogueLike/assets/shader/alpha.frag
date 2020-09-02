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
	c.rgb *= c.a;

	vec4 cc = colorV;
	cc.rgb *= cc.a;
	fragColor = c * cc;
}