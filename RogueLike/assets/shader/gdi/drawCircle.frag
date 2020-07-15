#version 150

#if GL_ES
precision mediump float;
#endif

out vec4 fragColor;

uniform vec2 center;
uniform float radius;
uniform vec4 color;
uniform float lineWidth;

void main()
{
	float d = length(gl_FragCoord.xy - center);
	d = abs(radius - d) / (lineWidth / 2.0);
	d = 1.0 - clamp(d, 0.0 , 1.0);
	//d = step(0.00001, d);
	d = smoothstep(0.0, 1.0, d);

	fragColor = vec4(color.rgb, color.a * d);
}