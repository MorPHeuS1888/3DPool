#version 330 core
out vec4 FragColor;

in vec3 fragPosition;   // posição do fragmento
in vec3 fragNormal;     // normal do fragmento
in vec3 vertexColor;    // cor do vértice (nova entrada)

uniform vec3 lightPos;  // posição da luz
uniform vec3 viewPos;   // posição da câmera
uniform vec3 lightColor; // cor da luz
uniform int useLighting; // controle de iluminação (1 = ativar, 0 = desativar)

void main() {
    if (useLighting == 1) {
        // Apenas luz ambiente
    vec3 ambient = 1.0 * lightColor;  // intensidade da luz ambiente (ajustável)

    // Resultado final da cor do fragmento (multiplicando pela cor do vértice)
    vec3 result = ambient * vertexColor;
    FragColor = vec4(result, 1.0);
    } else {
        // Sem iluminação, apenas a cor original do vértice
        FragColor = vec4(vertexColor, 1.0);
    }
}
