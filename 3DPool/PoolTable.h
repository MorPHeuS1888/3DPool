#define GLEW_STATIC
#pragma once
#include <GL/glew.h>
#include "Balls.h" 
#include "ModelManager.h"
 

class PoolTable : public PoolLibrary:: Renderable {
public:
    PoolTable();    
    ~PoolTable();   

    void setup();    
    void applySceneContext(const glm::mat4& view, const glm::mat4& projection,
        const glm::vec3& lightPos, const glm::vec3& viewPos,
        const glm::vec3& lightColor, bool useLighting) override;
    GLuint getShaderProgram() const override { return shaderProgram; }

private:
    GLuint shaderProgram;
    PoolLibrary::ModelManager model;
};