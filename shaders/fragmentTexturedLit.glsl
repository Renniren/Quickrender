#version 330 core
out vec4 FragColor;
uniform vec3 QR_LightColor, QR_AmbientLightColor;

uniform vec4 color;
uniform sampler2D tex;

in vec2 TextureCoordinates;


void main()
{
	
   FragColor = texture(tex, TextureCoordinates) * color;
}