﻿#include "Animation.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


Animation::Animation(Balls& balls, PoolTable& table)
    : m_balls(balls), m_table(table), m_totalRotationAngle(0.0f) { }

void Animation::Update(float deltaTime) {
    if (!m_isAnimating) return;

    glm::vec3 currentPos = m_balls.GetBallPosition(m_activeBall);
    glm::vec3 newPos = currentPos + (m_currentVelocity * deltaTime);


    // Calcular distância e ângulo de rotação
    float distance = glm::length(m_currentVelocity) * deltaTime;
    float circumference = 2.0f * glm::pi<float>() * 0.05f;
    float deltaAngle = (distance / circumference) * 2.0f * glm::pi<float>();
    m_totalRotationAngle += deltaAngle;

    // Aplicar rotação no eixo pré-definido
    glm::vec3 currentRot = m_balls.GetBallRotation(m_activeBall);
    glm::vec3 rotationDelta = m_rotationAxis * deltaAngle;
    glm::vec3 newRot = currentRot + rotationDelta;
    m_balls.SetBallRotation(m_activeBall, newRot);
    

    // Debug: Mostrar movimento
    std::cout << "Bola " << (m_activeBall + 1)
        << " | Velocidade: (" << m_currentVelocity.x << ", " << m_currentVelocity.z << ")"
        << " | Rotação do Eixo: (" << m_rotationAxis.x << ", " << m_rotationAxis.y << ", " << m_rotationAxis.z << ")"
        << " | Angulo de rotação: " << glm::degrees(m_totalRotationAngle) << " graus\n";

    if (CheckCollisions(newPos)) {
        std::cout << "Colisão foi detetada.\n";
        m_isAnimating = false;
        return;
    }

    m_balls.SetBallPosition(m_activeBall, newPos);

} 

void Animation::StartRandomAnimation() {
    if (m_isAnimating) {
        std::cout << "A animação já se está a decorrer\n";
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> ballDist(0, 14);

    m_activeBall = ballDist(gen);
    GenerateRandomDirection();

    // Calcular eixo de rotação inicial
    glm::vec3 moveDir = glm::normalize(m_currentVelocity);
    m_rotationAxis = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), moveDir));
    m_totalRotationAngle = 0.0f;

    std::cout << "A começar animação - Bola: " << m_activeBall
        << " | Direção: (" << m_currentVelocity.x << ", " << m_currentVelocity.z << ")\n";

    m_isAnimating = true;
}

bool Animation::CheckCollisions(const glm::vec3& newPosition) {
    // Debug de colisaõ com as  bordas d mesa
    if (newPosition.x < -0.7f) {
        std::cout << "Colisão com a parede esquerda\n";
        return true;
    }
    if (newPosition.x > 0.7f) {
        std::cout << "Colisão com a parede direita\n";
        return true;
    }
    if (newPosition.z < -1.4f) {
        std::cout << "Colisão com a parede de baixp\n";
        return true;
    }
    if (newPosition.z > 1.4f) {
        std::cout << "Colisão com a parede de cima\n";
        return true;
    }

    // Debug de colisão entre bolas
    const float ballRadius = 0.05f;
    for (int i = 0; i < 15; ++i) {
        if (i == m_activeBall) continue;
        glm::vec3 otherPos = m_balls.GetBallPosition(i);
        float distance = glm::distance(newPosition, otherPos);
        if (distance < (ballRadius * 2)) { 
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

    //Velocidade fixa
    m_currentVelocity *= 0.3f;

    // Debug: Mostrar direção gerada
    std::cout << "Direção: ("
        << m_currentVelocity.x << ", "
        << m_currentVelocity.z << ")\n";
}

bool Animation::IsAnimating() const {
    return m_isAnimating;
}