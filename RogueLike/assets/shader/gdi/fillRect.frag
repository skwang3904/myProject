#version 150

#if GL_ES
precision mediump float;
#endif

out vec4 fragColor;

uniform vec4 color;
uniform vec4 rect;
uniform float radius;
uniform float radian;

mat2 rotate = mat2(cos(radian) , -sin(radian), sin(radian), cos(radian));

vec2 translate(vec2 p, vec2 t)
{
	vec2 s = p - t;
	s = rotate * s;

	return s;
}

float boxDist(vec2 p, vec2 size, float radius)
{
	size -= vec2(radius);
	
	vec2 d = abs(p) - size;
  	return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - radius;
}


void main()
{
	vec2 uv = gl_FragCoord.xy;
	
	vec2 center = vec2( rect.x + rect.z / 2.0, rect.y - rect.w / 2.0f);
	vec2 size = rect.wz / 2.0;

	float rt1 = boxDist(translate(uv, center), size, radius);


	fragColor = vec4(color.rgb, color.a * (-rt1));
}