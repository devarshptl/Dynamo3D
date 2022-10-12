#version 150 core

in vec3 position;

out vec3 geomPosition;

uniform mat4 AspectRatioMatrix;
uniform mat4 MVPMatrix;
uniform mat4 ModelMatrix;

void main() {
    gl_Position = AspectRatioMatrix * MVPMatrix * vec4(position, 1.0);
    geomPosition = vec3(ModelMatrix * vec4(position, 1.0));
}
