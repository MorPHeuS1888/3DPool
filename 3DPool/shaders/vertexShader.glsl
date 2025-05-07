#version 330 core
layout(location = 0) in vec3 position;  // posição dos vértices
layout(location = 1) in vec3 normal;    // normal dos vértices
layout(location = 2) in vec3 color;     // cor dos vértices (nova entrada)

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPosition;
out vec3 fragNormal;
out vec3 vertexColor; // cor do vértice (nova saída)

void main() {
    fragPosition = vec3(model * vec4(position, 1.0));
    fragNormal = mat3(transpose(inverse(model))) * normal; // normal transformada
    vertexColor = color; // passa a cor do vértice para o fragment shader

    gl_Position = projection * view * vec4(fragPosition, 1.0);
}
