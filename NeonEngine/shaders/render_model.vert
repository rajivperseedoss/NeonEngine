#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat3 model_normals;
uniform mat4 model_view_projection;

uniform int expand_vertices;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = model_normals * aNormal;
    TexCoords = aTexCoords;
    vec3 pos = aPos;
    if (expand_vertices == 1) {
        pos += 0.05 * aNormal;
    }
    gl_Position = model_view_projection * vec4(pos, 1.0);
}