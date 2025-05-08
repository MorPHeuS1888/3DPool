#version 330 core
layout(location = 0) in vec3 position;  // posição dos vértices
layout(location = 1) in vec3 normal;    // normal dos vértices
layout(location = 2) in vec3 color;     // cor dos vértices

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPosition; // posição do fragmento no espaço do mundo
out vec3 fragNormal;   // normal do fragmento no espaço do mundo
out vec3 vertexColor;  // cor do vértice

void main() {
    // Calcula a posição do fragmento no espaço do mundo
    fragPosition = vec3(model * vec4(position, 1.0));
    
    // Transforma a normal para o espaço do mundo
    fragNormal = mat3(transpose(inverse(model))) * normal;
    
    // Passa a cor do vértice para o fragment shader
    vertexColor = color;

    // Calcula a posição final do vértice na tela
    gl_Position = projection * view * vec4(fragPosition, 1.0);
}
