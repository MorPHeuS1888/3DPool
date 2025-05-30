#include "Light.h"
#include <glm/gtc/type_ptr.hpp>

Light::Light() :
    ambientEnabled(true),
    directionalEnabled(false),
    pointEnabled(false),
    spotEnabled(false)
{
    // Incializa as propriedades das luzes
    directional.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    directional.color = glm::vec3(0.8f, 0.8f, 0.8f);

    point.position = glm::vec3(1.0f, 2.0f, 2.0f);
    point.color = glm::vec3(1.0f, 1.0f, 1.0f);
    point.constant = 1.0f;
    point.linear = 0.09f;
    point.quadratic = 0.032f;

    spot.position = glm::vec3(0.0f, 2.0f, 0.0f);
    spot.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    spot.color = glm::vec3(1.0f, 1.0f, 1.0f);
    spot.cutOff = glm::cos(glm::radians(12.5f));
    spot.outerCutOff = glm::cos(glm::radians(17.5f));
    spot.constant = 1.0f;
    spot.linear = 0.09f;
    spot.quadratic = 0.032f;
}

void Light::setup(GLuint shaderProgram) {
    glUseProgram(shaderProgram);
    
    // Defini��es iniciais das luzes
    glUniform1i(glGetUniformLocation(shaderProgram, "ambientEnabled"), ambientEnabled);
    glUniform1i(glGetUniformLocation(shaderProgram, "directionalEnabled"), directionalEnabled);
    glUniform1i(glGetUniformLocation(shaderProgram, "pointEnabled"), pointEnabled);
    glUniform1i(glGetUniformLocation(shaderProgram, "spotEnabled"), spotEnabled);
}

void Light::update(GLuint shaderProgram, const glm::vec3& viewPos) {
    glUseProgram(shaderProgram);

    // Luz ambiente
    glUniform1i(glGetUniformLocation(shaderProgram, "ambientEnabled"), ambientEnabled);
    glUniform3f(glGetUniformLocation(shaderProgram, "ambientLight"), 0.8f, 0.8f, 0.8f);

    // Luz direcional
    glUniform1i(glGetUniformLocation(shaderProgram, "directionalEnabled"), directionalEnabled);
    if (directionalEnabled) {
        glUniform3fv(glGetUniformLocation(shaderProgram, "directionalLight.direction"), 1, glm::value_ptr(directional.direction));
        glUniform3fv(glGetUniformLocation(shaderProgram, "directionalLight.color"), 1, glm::value_ptr(directional.color));
    }

    // Luz pontual
    glUniform1i(glGetUniformLocation(shaderProgram, "pointEnabled"), pointEnabled);
    if (pointEnabled) {
        glUniform3fv(glGetUniformLocation(shaderProgram, "pointLight.position"), 1, glm::value_ptr(point.position));
        glUniform3fv(glGetUniformLocation(shaderProgram, "pointLight.color"), 1, glm::value_ptr(point.color));
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLight.constant"), point.constant);
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLight.linear"), point.linear);
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLight.quadratic"), point.quadratic);
    }

    // Luz c�nica 
    glUniform1i(glGetUniformLocation(shaderProgram, "spotEnabled"), spotEnabled);
    if (spotEnabled) {
        // Posi��o da luz = posi��o da c�mera
        glUniform3fv(glGetUniformLocation(shaderProgram, "spotLight.position"), 1, glm::value_ptr(viewPos));

        // Dire��o fixa para frente (eixo -Z, ajuste conforme a orienta��o da c�mera)
        glm::vec3 spotDirection = glm::vec3(0.0f, 0.0f, -1.0f); // Ajuste o Y para apontar para a mesa
        glUniform3fv(glGetUniformLocation(shaderProgram, "spotLight.direction"), 1, glm::value_ptr(spotDirection));

        // Par�metros da luz
        glUniform3fv(glGetUniformLocation(shaderProgram, "spotLight.color"), 1, glm::value_ptr(spot.color));
        glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.cutOff"), spot.cutOff);
        glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.outerCutOff"), spot.outerCutOff);
        glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.constant"), spot.constant);
        glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.linear"), spot.linear);
        glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.quadratic"), spot.quadratic);
    }
}

void Light::toggleAmbient() {
    ambientEnabled = !ambientEnabled;
}

void Light::toggleDirectional() {
    directionalEnabled = !directionalEnabled;
}

void Light::togglePoint() {
    pointEnabled = !pointEnabled;
}

void Light::toggleSpot() {
    spotEnabled = !spotEnabled;
}