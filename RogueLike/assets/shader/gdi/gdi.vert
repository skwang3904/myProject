#version 150

#if GL_ES
precision mediump float;
#endif

in vec4 position;
uniform mat4 mProjection, mModelview;

void main()
{
	gl_Position = mProjection *  mModelview * position;
}