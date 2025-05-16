#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture_diffuse;
uniform vec3 viewPos;
uniform int useLighting;
uniform int useTexture;
uniform int lightType; // 0 = pontual, 1 = direcional

// Luz pontual
uniform vec3 pointLightPos;
uniform vec3 lightColor;

// Luz direcional
struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionalLight dirLight;

void main() {
    vec4 color = (useTexture == 1) ? texture(texture_diffuse, TexCoord) : vec4(0.0, 0.6, 0.0, 1.0);

    if (useLighting == 1) {
        vec3 norm = normalize(Normal);
        vec3 ambient, diffuse, specular;

        if (lightType == 0) {
            // Luz pontual
            vec3 lightDir = normalize(pointLightPos - FragPos);

            float ambientStrength = 0.2;
            ambient = ambientStrength * lightColor;

            float diff = max(dot(norm, lightDir), 0.0);
            diffuse = diff * lightColor;

            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            specular = lightColor * spec;

        } else {
            // Luz direcional
            vec3 lightDir = normalize(-dirLight.direction);

            ambient = dirLight.ambient;

            float diff = max(dot(norm, lightDir), 0.0);
            diffuse = diff * dirLight.diffuse;

            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            specular = dirLight.specular * spec;
        }

        vec3 result = clamp((ambient + diffuse + specular) * color.rgb, 0.0, 1.0);
        FragColor = vec4(result, color.a);
    } else {
        FragColor = color;
    }
}
