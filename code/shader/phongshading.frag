#version 150 core

in vec3 fragNormal;
in vec3 fragPosition;

out vec4 outColor;

//shadow
uniform samplerCube depthMap;
uniform float far_plane;
uniform bool red_shadow;

//lighting
uniform int lighting_strategy;
uniform vec3 color;
uniform vec3 eyePosition;
uniform vec3 lightPosition; 
uniform samplerCube skybox;

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculation(vec3 fragPos) {
    vec3 fragToLight = fragPos - lightPosition;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.22;
    int samples = 20;

    float viewDistance = length(eyePosition - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i) {
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
        
    return shadow;
}

void phongLighting() {
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 lightDir = normalize(lightPosition - fragPosition);
    float diff = max(dot(fragNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * 2;

    // specular
    float specularStrength = 0.7;
    vec3 viewDir = normalize(eyePosition - fragPosition);
    vec3 reflectDir = reflect(-lightDir, fragNormal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    float shadow = ShadowCalculation(fragPosition);

    vec3 result;
    if (red_shadow && shadow != 0.0) {
        result = vec3(1.0, 0.0, 0.0);
    } else {
        result = (ambient + (1 - shadow) * (diffuse + specular)) * color;
    }
    outColor = vec4(result, 1.0);
}

void mirrorLighting() {
    // ambient
    vec3 ambient = 0.8 * vec3(1.0, 1.0, 1.0);

    vec3 I = normalize(fragPosition - eyePosition);
    vec3 R = reflect(I, normalize(fragNormal));
    vec3 texture_color = texture(skybox, R).rgb;

    float shadow = ShadowCalculation(fragPosition);

    vec3 result;
    if (red_shadow && shadow != 0.0) {
        result = vec3(1.0, 0.0, 0.0);
    } else {
        result = (ambient + (1 - shadow)) * texture_color;
    }
    outColor = vec4(result, 1.0);
}

void refractLighting() {
    // ambient
    vec3 ambient = 0.6 * vec3(1.0, 1.0, 1.0);

    float ratio = 1.00 / 2.42;
    vec3 I = normalize(fragPosition - eyePosition);
    vec3 R = refract(I, normalize(fragNormal), ratio);
    vec3 texture_color = texture(skybox, R).rgb;

    float shadow = ShadowCalculation(fragPosition);

    vec3 result;
    if (red_shadow && shadow != 0.0) {
        result = vec3(1.0, 0.0, 0.0);
    } else {
        result = (ambient + (1 - shadow)) * texture_color;
    }
    outColor = vec4(result, 1.0);
}

void main() {
    if (lighting_strategy == 1) {
        phongLighting();
    } else if (lighting_strategy == 2) {
        mirrorLighting();
    } else if (lighting_strategy == 3) {
        refractLighting();
    }
}
