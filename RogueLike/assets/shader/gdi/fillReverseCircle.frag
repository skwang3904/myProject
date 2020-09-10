#version 150

#if GL_ES
precision mediump float;
#endif

out vec4 fragColor;

uniform vec2 center;
uniform float radius;
uniform vec4 color;

void main()
{
	float d = length(gl_FragCoord.xy - center);
	d = clamp(d - radius, 0.0, 1.0);

	fragColor = vec4(color.rgb, color.a * d);
}