#include "Balls.h"
#include "ShaderUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include "ModelManager.h"


Balls::Balls() : shaderProgram(0) {}

Balls::~Balls() { }

void Balls::setup() {
    ballPositions.resize(15);
    ballVelocities.resize(15, glm::vec3(0.0f));

    ShaderInfo shaders[] = {
        { GL_VERTEX_SHADER, "shaders/vertexShader.ver" },
        { GL_FRAGMENT_SHADER, "shaders/fragmentShader.frag" },
        { GL_NONE, NULL }
    };

    shaderProgram = LoadShaders(shaders);
    if (shaderProgram == 0) {
        std::cerr << "Erro ao criar shader program para Balls\n";
        return;
    }

   std::srand(static_cast<unsigned int>(time(nullptr)));

    const float tableMinX = -0.7f;
    const float tableMaxX = 0.7f;
    const float tableMinZ = -1.3f;
    const float tableMaxZ = 1.3f;
    const float minDistance = 0.1f;

    std::vector<glm::vec3> positions;

    auto isPositionValid = [&](const glm::vec3& newPos) {
        for (const auto& pos : positions) {
            if (glm::distance(newPos, pos) < minDistance) {
                return false;
            }
        }
        return true;
        };

    for (int i = 0; i < 15; ++i) {
        glm::vec3 newPos;
        do {
            float x = tableMinX + static_cast<float>(std::rand()) /
                (static_cast<float>(RAND_MAX / (tableMaxX - tableMinX)));

            float z = tableMinZ + static_cast<float>(std::rand()) /
                (static_cast<float>(RAND_MAX / (tableMaxZ - tableMinZ)));

            newPos = glm::vec3(x, 0.05f, z);
        } while (!isPositionValid(newPos));

        positions.push_back(newPos);
    }

    for (int index = 0; index < 15; ++index) {
        Ball ball;
        std::string objPath = "PoolBalls/Ball" + std::to_string(index + 1) + ".obj";
        if (ball.model.Load(objPath)) {
            ball.model.Install();
            ball.model.BindShaderAttributes(shaderProgram);
            ball.position = positions[index];

            // Rotação inicial 
            ball.initialRotation = glm::vec3(
                glm::radians(180.0f),
                glm::radians(static_cast<float>(rand() % 360)),
                glm::radians(static_cast<float>(rand() % 360))
            );

            // Inicializa rotações
            ball.movementRotation = glm::vec3(0.0f);
            ball.rotation = ball.initialRotation;

            balls.push_back(ball);
        }
    }


}

//atualiza a posição de uma bola específica
void Balls::SetBallPosition(int index, const glm::vec3& pos) {
    if (index >= 0 && index < balls.size()) {
        balls[index].position = pos; // Atualiza a posição de renderização
        balls[index].model.SetScale(glm::vec3(0.05f)); 
    }
}

//retorna a posição de uma bola específica
glm::vec3 Balls::GetBallPosition(int index) const {
    if (index >= 0 && index < balls.size()) {
        return balls[index].position;
    }
    return glm::vec3(0.0f);
}

//retorna a rotação de uma bola específica
glm::vec3 Balls::GetBallRotation(int index) const {
    if (index >= 0 && index < balls.size()) {
        return balls[index].rotation;
    }
    return glm::vec3(0.0f);
}

//define a rotação de uma bola específica
void Balls::SetBallRotation(int index, const glm::vec3& rotation) {
    if (index >= 0 && index < balls.size()) {
        balls[index].rotation = rotation;
    }
}




void Balls::applySceneContext(const glm::mat4& view, const glm::mat4& projection,
    const glm::vec3& lightPos, const glm::vec3& viewPos,
    const glm::vec3& lightColor, bool useLighting) {
    glUseProgram(shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(viewPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));


    glUniform1i(glGetUniformLocation(shaderProgram, "useLighting"), useLighting ? 1 : 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);

    for (auto& ball : balls) {
        ball.model.SetScale(glm::vec3(0.05f));
        ball.model.Render(ball.position, ball.rotation);
        glDisable(GL_POLYGON_OFFSET_FILL);
    }
}
