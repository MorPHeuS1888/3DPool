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