#version 330 core
out vec4 FragColor;

in vec3 fragPosition;   // posição do fragmento
in vec3 fragNormal;     // normal do fragmento
in vec3 vertexColor;    // cor do vértice (nova entrada)

uniform vec3 lightPos;  // posição da luz
uniform vec3 viewPos;   // posição da câmera
uniform vec3 lightColor; // cor da luz

void main() {
    // Normaliza as normais
    vec3 norm = normalize(fragNormal);

    // Vetor para a luz
    vec3 lightDir = normalize(lightPos - fragPosition);

    // Cálculo da intensidade da luz (modelo Phong básico)
    float diff = max(dot(norm, lightDir), 0.0);  // luz difusa
    vec3 diffuse = diff * lightColor;  // cor difusa

    // Cálculo da luz especular (simples)
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);  // brilho especular
    vec3 specular = spec * lightColor;  // cor especular

    // Combinando a luz ambiente, difusa e especular
    vec3 ambient = 0.1 * lightColor;  // luz ambiente (escura, mas presente)

    // Resultado final da cor do fragmento (multiplicando pela cor do vértice)
    vec3 result = (ambient + diffuse + specular) * vertexColor;
    FragColor = vec4(result, 1.0);
}
