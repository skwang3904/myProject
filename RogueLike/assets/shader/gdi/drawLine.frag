#version 150

#if GL_ES
precision mediump float;
#endif

out vec4 fragColor;

uniform vec4 color;
uniform vec2 sp;
uniform vec2 ep;
uniform float lineWidth;

void main()
{
	vec2 uv = gl_FragCoord.xy;
	float lw = lineWidth / 2.0;
	vec2 n = ep - sp;
	float len = length(n);
	n /= len;

	vec2 m = uv - sp;
	vec2 proj = n * clamp(dot(m,n), 0.0, len);
	float dis = length(m - proj) - lw;
	
	float d = clamp(-dis, 0.0, 1.0);

	fragColor = vec4(color.rgb, d);
}
