#version 150

#if GL_ES
precision mediump float;
#endif

out vec4 fragColor;

uniform vec2 center;
uniform vec2 size;
uniform float radian;
uniform vec4 color;

mat2 rotate = mat2(cos(radian) , sin(radian), -sin(radian), cos(radian));

float dise(vec2 p, vec2 size)
{
    vec2 s = 1.0 / size;
    return (length(p * s) - 1.0)/ length(p * s * s);
}

void main()
{
    vec2 a = rotate * (gl_FragCoord.xy - center);

	vec2 s = size / 2.0;
#if 0
	float d = length(a / s);
#elif 0 
	a = vec2(a.x * a.x, a.y * a.y);
	float x = s.x * s.x;
	float y = s.y * s.y;
	
	float d = a.x/x + a.y/y;
#else
	float d = dise(a,s);
#endif
	d = clamp(d, 0.0, 1.0);

	fragColor = vec4(color.rgb, color.a * (1.0 - d));
}