#version 150 core

in vec3 position;

out vec3 texture_coordinate;

uniform mat4 AspectRatioMatrix;
uniform mat4 VPMatrix;

void main()
{
    texture_coordinate = position;
    vec4 pos = AspectRatioMatrix * VPMatrix * vec4(position, 1.0);
    gl_Position = pos.xyww;
}  
