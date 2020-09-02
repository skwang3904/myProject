#version 150

#if GL_ES
precision mediump float;
#endif

out vec4 fragColor;

uniform vec4 color;
uniform vec4 rect;
uniform float radius;
uniform float lineWidth;

vec2 translate(vec2 p, vec2 t)
{
	return p - t;
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
	
	vec2 center = vec2(rect.x + rect.z / 2.0, rect.y - rect.w / 2.0);
	vec2 size = vec2(rect.zw / 2.0);

	float rt1 = boxDist(translate(uv, center), size, radius);
	float rt2 = boxDist(translate(uv, center), (size - lineWidth), radius);

	float a = max(rt1, -rt2);

	fragColor = vec4(color.rgb, color.a * (-a) + 1.0);
}