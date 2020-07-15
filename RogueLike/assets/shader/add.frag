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
	//glblendfunc(gl_src_alpha, gl_one)
	vec2 size = textureSize(texBase, 0);
	vec2 uv = gl_FragCoord.xy / size;
	vec4 base = texture(texBase, uv);

	vec4 blend = texture(texBlend, texCoordV);
    
	fragColor = vec4(base.rgb + blend.rgb * blend.a, base.a);
}