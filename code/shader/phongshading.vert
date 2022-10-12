#version 150 core

in vec3 position;
in vec3 vertex_normal;

out vec3 fragPosition;
out vec3 fragNormal;

uniform mat4 AspectRatioMatrix;
uniform mat4 MVPMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

void main() {
    gl_Position = AspectRatioMatrix * MVPMatrix * vec4(position, 1.0);
    fragPosition = vec3(ModelMatrix * vec4(position, 1.0));
    fragNormal = normalize(NormalMatrix * vertex_normal);
}
