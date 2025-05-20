#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture_diffuse;
uniform vec3 viewPos;
uniform int useTexture;

// Light structs
struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

// Light uniforms
uniform bool ambientEnabled;
uniform vec3 ambientLight;

uniform bool directionalEnabled;
uniform DirectionalLight directionalLight;

uniform bool pointEnabled;
uniform PointLight pointLight;

uniform bool spotEnabled;
uniform SpotLight spotLight;

void main() {
    vec4 color;
    if (useTexture == 1) {
        color = texture(texture_diffuse, TexCoord);
    } else {
        color = vec4(0.0, 0.6, 0.0, 1.0); // Verde para a mesa
    }

    vec3 lighting = vec3(0.0);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Ambient lighting
    if (ambientEnabled) {
        lighting += ambientLight * color.rgb;
    }

    // Directional lighting
    if (directionalEnabled) {
        vec3 lightDir = normalize(-directionalLight.direction);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * directionalLight.color;
        lighting += diffuse * color.rgb;
    }

    // Point light
    if (pointEnabled) {
        vec3 lightDir = normalize(pointLight.position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * pointLight.color;
        
        // Attenuation
        float distance = length(pointLight.position - FragPos);
        float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + 
                                 pointLight.quadratic * (distance * distance));
        
        diffuse *= attenuation;
        lighting += diffuse * color.rgb;
    }

    // Spot light
    if (spotEnabled) {
        vec3 lightDir = normalize(spotLight.position - FragPos);
        float theta = dot(lightDir, normalize(-spotLight.direction));
        float epsilon = spotLight.cutOff - spotLight.outerCutOff;
        float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);
        
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * spotLight.color;
        
        // Attenuation
        float distance = length(spotLight.position - FragPos);
        float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + 
                                  spotLight.quadratic * (distance * distance));
        
        diffuse *= attenuation * intensity;
        lighting += diffuse * color.rgb;
    }

    vec3 result = lighting * color.rgb;
    FragColor = vec4(result, color.a);
}