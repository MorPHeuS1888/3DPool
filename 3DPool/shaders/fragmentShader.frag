#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture_diffuse;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform int useLighting;
uniform int useTexture;

void main() {
    vec4 color;
    if (useTexture == 1) {
        color = texture(texture_diffuse, TexCoord);
    } else {
        color = vec4(0.0, 0.6, 0.0, 1.0); // Verde para a mesa
    }

    if (useLighting == 1) {
        // Luz ambiente
        float ambientStrength = 0.2;
        vec3 ambient = ambientStrength * lightColor;
        
        // Luz difusa
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        
        // Combinação
       vec3 result = clamp((ambient + diffuse) * color.rgb, 0.0, 1.0);
        FragColor = vec4(result, color.a);
    } else {
        FragColor = color;
    }
}