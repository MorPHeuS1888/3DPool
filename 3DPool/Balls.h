// Balls.h
#pragma once
#include "Renderable.h"
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "light.h"
#include <string> 

namespace PoolLibrary {
    class ModelManager {
    public:
        struct ModelData {
            GLuint VAO;
            GLuint VBO;
            GLuint textureID;
            int vertexCount;
            std::vector<float> vertices;
            std::string texturePath;
        };

        bool Load(const std::string& obj_model_filepath);
        void Install(void);
        void Render(const glm::vec3& position, const glm::vec3& orientation);
        void BindShaderAttributes(GLuint shaderProgram);

        void SetVertices(const std::vector<float>& vertices) {
            modelData.vertices = vertices;
            modelData.vertexCount = vertices.size() / 8; 
        }

        ModelData& GetModelData() { return modelData; }

    private:
        ModelData modelData;
        void loadMaterials(const std::string& mtlPath);
    };
}

class Balls : public Renderable {
public:
    Balls();
    ~Balls();

    void setup();
    void render(const glm::mat4& view, const glm::mat4& projection,
        const glm::vec3& lightPos, const glm::vec3& viewPos,
        const glm::vec3& lightColor, bool useLighting) override;
    GLuint getShaderProgram() const override { return shaderProgram; }

private:
    GLuint shaderProgram;
    struct Ball {
        PoolLibrary::ModelManager model;
        glm::vec3 position;
        glm::vec3 rotation; // Adicionado para armazenar rotação
    };

    std::vector<Ball> balls;

};