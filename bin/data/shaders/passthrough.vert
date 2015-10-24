#version 330

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;

out vec2 vs_texcoord;

void main()
{
    vs_texcoord = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}