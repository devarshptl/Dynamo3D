#version 150 core

out vec4 outColor;

in vec3 texture_coordinate;

uniform samplerCube skybox;

void main()
{    
    outColor = texture(skybox, texture_coordinate);
}
