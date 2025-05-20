#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <GL/glew.h>

class Light {
public:
    Light();

    void setup(GLuint shaderProgram);
    void update(GLuint shaderProgram, const glm::vec3& viewPos);
    void toggleAmbient();
    void toggleDirectional();
    void togglePoint();
    void toggleSpot();
    bool isAmbientEnabled() const { return ambientEnabled; }
    bool isDirectionalEnabled() const { return directionalEnabled; }
    bool isPointEnabled() const { return pointEnabled; }
    bool isSpotEnabled() const { return spotEnabled; }

private:
    struct LightParams {
        glm::vec3 direction;
        glm::vec3 position;
        glm::vec3 color;
        float cutOff;
        float outerCutOff;
        float constant;
        float linear;
        float quadratic;
    };

    bool ambientEnabled;
    bool directionalEnabled;
    bool pointEnabled;
    bool spotEnabled;


    LightParams directional;
    LightParams point;
    LightParams spot;
};

#endif // LIGHT_H