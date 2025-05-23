#include "Animation.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


Animation::Animation(Balls& balls, PoolTable& table)
    : m_balls(balls), m_table(table) {
    std::cout << "Animation System Initialized\n";
}

void Animation::Update(float deltaTime) {
    if (!m_isAnimating) return;

    glm::vec3 currentPos = m_balls.GetBallPosition(m_activeBall);
    glm::vec3 newPos = currentPos + (m_currentVelocity * deltaTime);


        // 1. Normalizar direção do movimento
        glm::vec3 moveDir = glm::normalize(m_currentVelocity);

        // 2. Calcular eixo de rotação (perpendicular ao movimento e ao eixo Y)
        glm::vec3 rotationAxis = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), moveDir));

        // 3. Calcular ângulo de rotação proporcional à distância percorrida
        float distance = glm::length(m_currentVelocity) * deltaTime;
        float circumference = 2.0f * glm::pi<float>() * 0.05f; // Circunferência da bola
        float rotationAngle = (distance / circumference) * 2.0f * glm::pi<float>();

        // 4. Atualizar rotação
        glm::vec3 currentRot = m_balls.GetBallRotation(m_activeBall);
        glm::vec3 newRot = currentRot + rotationAxis * rotationAngle;
        m_balls.SetBallRotation(m_activeBall, newRot);
    

    // Debug: Mostrar movimento
    std::cout << "Ball " << m_activeBall
        << " | Velocity: (" << m_currentVelocity.x << ", " << m_currentVelocity.z << ")"
        << " | Rotation Axis: (" << rotationAxis.x << ", " << rotationAxis.y << ", " << rotationAxis.z << ")"
        << " | Rotation Angle: " << rotationAngle << " degrees\n";

    if (CheckCollisions(newPos)) {
        std::cout << "Collision Detected! Stopping animation.\n";
        m_isAnimating = false;
        return;
    }

    m_balls.SetBallPosition(m_activeBall, newPos);

} 

void Animation::StartRandomAnimation() {
    if (m_isAnimating) {
        std::cout << "Animation already running\n";
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> ballDist(0, 14);

    m_activeBall = ballDist(gen);
    GenerateRandomDirection();

    std::cout << "Starting animation - Ball: " << m_activeBall
        << " | Direction: (" << m_currentVelocity.x << ", " << m_currentVelocity.z << ")\n";

    m_isAnimating = true;
}

bool Animation::CheckCollisions(const glm::vec3& newPosition) {
    // Debug de bordas
    if (newPosition.x < -0.7f) {
        std::cout << "Collision with LEFT wall\n";
        return true;
    }
    if (newPosition.x > 0.7f) {
        std::cout << "Collision with RIGHT wall\n";
        return true;
    }
    if (newPosition.z < -1.4f) {
        std::cout << "Collision with BOTTOM wall\n";
        return true;
    }
    if (newPosition.z > 1.4f) {
        std::cout << "Collision with TOP wall\n";
        return true;
    }

    // Debug de colisão entre bolas
    const float ballRadius = 0.05f;
    for (int i = 0; i < 15; ++i) {
        if (i == m_activeBall) continue;
        glm::vec3 otherPos = m_balls.GetBallPosition(i);
        float distance = glm::distance(newPosition, otherPos);
        if (distance < (ballRadius * 2)) { // 0.1 unidades de distância
            return true;
        }
    }
    return false;
}


void Animation::GenerateRandomDirection() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * glm::pi<float>());

    // Gera direção via ângulo (evita vetor zero)
    float angle = angleDist(gen);
    m_currentVelocity = glm::vec3(
        glm::cos(angle),
        0.0f,
        glm::sin(angle)
    );

    // Define velocidade fixa (ex: 3.0 unidades/segundo)
    m_currentVelocity *= 0.1f;

    // Debug: Verifique valores
    std::cout << "Generated Direction: ("
        << m_currentVelocity.x << ", "
        << m_currentVelocity.z << ")\n";
}

bool Animation::IsAnimating() const {
    return m_isAnimating;
}