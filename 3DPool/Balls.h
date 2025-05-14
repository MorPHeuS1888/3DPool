#pragma once
#include "Renderable.h"
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string> 

class Balls : public Renderable {
public:
    Balls();
    ~Balls();

    void setup();
    void render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& lightPos, const glm::vec3& viewPos, const glm::vec3& lightColor, bool useLighting) override;

private:
    struct Ball {
        GLuint VAO;
        GLuint VBO;
        GLuint textureID;
        int vertexCount;
        glm::vec3 position;
    };

    std::vector<Ball> balls;
    GLuint shaderProgram;

    void loadBallModel(const std::string& path, Ball& ball);
    void createTriangleFormation(std::vector<glm::vec3>& positions);
};