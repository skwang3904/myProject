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
	fragColor = texture(texBlend, texCoordV) * colorV;

	//fragColor = texture(texBase, texCoordV) * colorV;

	//fragColor =  vec4(1,1,1,1);
}