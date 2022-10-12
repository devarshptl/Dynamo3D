#version 150 core

in vec3 position;
out vec3 p;

uniform mat4 AspectRatioMatrix;
uniform mat4 MVPMatrix;

void main()
{
    gl_Position = AspectRatioMatrix * MVPMatrix * vec4(position, 1.0);
    p = position;
}
