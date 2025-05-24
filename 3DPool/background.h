#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "ModelManager.h"
#include "ShaderUtils.h"

class Background : public PoolLibrary::Renderable {
public:
    Background();
    ~Background();

    bool Load();
    void applySceneContext(const glm::mat4& view, const glm::mat4& projection,
        const glm::vec3& lightPos, const glm::vec3& viewPos,
        const glm::vec3& lightColor, bool useLighting) override;
    GLuint getShaderProgram() const override { return shaderProgram; }

private:
    GLuint shaderProgram;
    GLuint cubemapTexture;
    PoolLibrary::ModelManager model;

    GLuint loadCubemap();
    void setupGeometry();
};