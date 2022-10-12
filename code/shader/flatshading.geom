#version 150 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 geomPosition[];

out vec3 face_normal;
out vec3 fragPosition;

// don't need normal matrix because we calculate normal with world space

vec3 GetNormal() {
   vec3 a = geomPosition[1] - geomPosition[0];
   // vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = geomPosition[2] - geomPosition[1];
   return normalize(cross(a, b));
} 

void main() {
    face_normal = GetNormal();

    int i;
    for(i = 0; i < gl_in.length(); i++) {
        gl_Position = gl_in[i].gl_Position;
        fragPosition = geomPosition[i];
        EmitVertex();
    }

    EndPrimitive();
}
