#pragma once
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
            glm::vec3 scale = glm::vec3(1.0f);
        };

        bool Load(const std::string& obj_model_filepath);
        void Install(void);
        void Render(const glm::vec3& position, const glm::vec3& orientation);
        void BindShaderAttributes(GLuint shaderProgram);
        void SetScale(const glm::vec3& scale) { modelData.scale = scale; }
        void SetVertices(const std::vector<float>& vertices) {
            modelData.vertices = vertices;
            modelData.vertexCount = vertices.size() / 8;
        }

        ModelData& GetModelData() { return modelData; }

    private:
        ModelData modelData;
        void loadMaterials(const std::string& mtlPath);
    };

    class Renderable {
    public:
        virtual ~Renderable() = default;
        virtual GLuint getShaderProgram() const = 0;
        virtual void applySceneContext(const glm::mat4& view, const glm::mat4& projection,
            const glm::vec3& lightPos, const glm::vec3& viewPos,
            const glm::vec3& lightColor, bool useLighting) = 0;
    };

}