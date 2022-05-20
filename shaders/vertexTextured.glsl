#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoord;

out vec2 TextureCoordinates;

uniform vec4 origvec;
uniform mat4 model, view, projection, mvp;
void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	//gl_Position = mvp * vec4(aPos, 1.0);
	TextureCoordinates = aTextureCoord;
}