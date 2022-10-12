#version 330 core
in vec3 position;

uniform mat4 ModelMatrix;

void main()
{
    gl_Position = ModelMatrix * vec4(position, 1.0);
}
