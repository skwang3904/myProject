#version 150

#if GL_ES
precision mediump float;
#endif

out vec2 texCoordV;
out vec4 colorV;

in vec4 position;
in vec2 texCoord;
in vec4 color;

uniform mat4 mProjection, mModelview;

void main()
{
	gl_Position = mProjection * mModelview * position;
	
	colorV = color / 255.0;
	texCoordV = texCoord;
}