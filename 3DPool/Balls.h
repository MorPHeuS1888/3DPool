#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "light.h"
#include <string> 
#include "ModelManager.h" 


class Balls : public PoolLibrary :: Renderable {
public:
    Balls();
    ~Balls();

    void setup();
    void applySceneContext(const glm::mat4& view, const glm::mat4& projection,
        const glm::vec3& lightPos, const glm::vec3& viewPos,
        const glm::vec3& lightColor, bool useLighting) override;
    GLuint getShaderProgram() const override { return shaderProgram; }
    size_t GetBallCount() const { return 15; } 
    glm::vec3 GetBallRotation(int index) const;
    void SetBallRotation(int index, const glm::vec3& rotation);
    glm::vec3 GetBallPosition(int index) const;
    void SetBallPosition(int index, const glm::vec3& pos);


private:
    GLuint shaderProgram;
    struct Ball {
        PoolLibrary::ModelManager model;
        glm::vec3 position;
        glm::vec3 rotation;       // Rotação total (inicial + movimento)
        glm::vec3 initialRotation; // Rotação inicial fixa
        glm::vec3 movementRotation; // Rotação acumulada do movimento

    };
    std::vector<glm::vec3> ballPositions;
    std::vector<glm::vec3> ballVelocities;
    std::vector<Ball> balls;

};