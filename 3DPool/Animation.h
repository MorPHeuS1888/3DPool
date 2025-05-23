#pragma once
#include "Balls.h"
#include "PoolTable.h"
#include <glm/glm.hpp>
#include <random>
#include <iostream> 

class Animation {
public:
    Animation(Balls& balls, PoolTable& table);
    void Update(float deltaTime);
    void StartRandomAnimation();
    bool IsAnimating() const;

private:
    Balls& m_balls;
    PoolTable& m_table;
    bool m_isAnimating = false;
    int m_activeBall = -1;
    glm::vec3 m_currentVelocity;

    bool CheckCollisions(const glm::vec3& newPosition);
    void GenerateRandomDirection();
};