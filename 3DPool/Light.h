#pragma once

//Luz pontual, direcionar e cónica necessario fazer

// lighting.h
#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

#endif // LIGHT_H