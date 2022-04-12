#version 330 core
out vec4 FragColor;
uniform vec4 color;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;

in vec2 TextureCoordinates;

void main()
{
   FragColor = texture(texture_diffuse1, TextureCoordinates) * color;
}