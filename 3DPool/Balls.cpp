// Balls.cpp
#include "Balls.h"
#include "ShaderUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace PoolLibrary {
    bool ModelManager::Load(const std::string& obj_model_filepath) {
        std::ifstream file(obj_model_filepath);
        if (!file.is_open()) {
            std::cerr << "Erro ao abrir: " << obj_model_filepath << std::endl;
            return false;
        }

        std::vector<glm::vec3> temp_vertices;
        std::vector<unsigned int> indices;
        std::string mtlPath;
        std::string line;

        size_t lastSlash = obj_model_filepath.find_last_of("\\/");
        std::string directory = (lastSlash != std::string::npos)
            ? obj_model_filepath.substr(0, lastSlash + 1)
            : "";

        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string prefix;
            ss >> prefix;

            if (prefix == "mtllib") {
                ss >> mtlPath;
                mtlPath = directory + mtlPath;
                loadMaterials(mtlPath);
            }
            else if (prefix == "v") {
                glm::vec3 v;
                ss >> v.x >> v.y >> v.z;
                temp_vertices.push_back(v);
            }
            else if (prefix == "f") {
                for (int i = 0; i < 3; ++i) {
                    std::string vertexStr;
                    ss >> vertexStr;
                    size_t pos = vertexStr.find('/');
                    unsigned int idx = static_cast<unsigned int>(std::stoi(vertexStr.substr(0, pos))) - 1;
                    indices.push_back(idx);
                }
            }
        }

        modelData.vertices.clear();
        for (unsigned int idx : indices) {
            const glm::vec3& v = temp_vertices[idx];

            // Position
            modelData.vertices.push_back(v.x);
            modelData.vertices.push_back(v.y);
            modelData.vertices.push_back(v.z);

            // Normal (sphere-based)
            glm::vec3 n = glm::normalize(v);
            modelData.vertices.push_back(n.x);
            modelData.vertices.push_back(n.y);
            modelData.vertices.push_back(n.z);

            // UV coordinates
            float u = 0.5f + std::atan2(v.z, v.x) / (2 * glm::pi<float>());
            float v_tex = 0.5f - std::asin(v.y) / glm::pi<float>();
            modelData.vertices.push_back(u);
            modelData.vertices.push_back(v_tex);
        }

        modelData.vertexCount = static_cast<int>(indices.size());
        return true;
    }

    void ModelManager::loadMaterials(const std::string& mtlPath) {
        std::ifstream mtlFile(mtlPath);
        if (!mtlFile.is_open()) {
            std::cerr << "Erro ao abrir .mtl: " << mtlPath << std::endl;
            return;
        }

        std::string line;
        while (std::getline(mtlFile, line)) {
            std::istringstream ss(line);
            std::string tag;
            ss >> tag;
            if (tag == "map_Kd") {
                std::string texFile;
                ss >> texFile;
                size_t lastSlash = mtlPath.find_last_of("\\/");
                std::string directory = (lastSlash != std::string::npos)
                    ? mtlPath.substr(0, lastSlash + 1)
                    : "";
                modelData.texturePath = directory + texFile;

                // Carregar textura com flip correto
                stbi_set_flip_vertically_on_load(false); // Já corrigimos nas UVs
     
            }
        }
    }

    void ModelManager::Install() {
        glGenVertexArrays(1, &modelData.VAO);
        glGenBuffers(1, &modelData.VBO);

        glBindVertexArray(modelData.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, modelData.VBO);
        glBufferData(GL_ARRAY_BUFFER, modelData.vertices.size() * sizeof(float),
            modelData.vertices.data(), GL_STATIC_DRAW);

        // Load texture if available
        if (!modelData.texturePath.empty()) {
            glGenTextures(1, &modelData.textureID);
            glBindTexture(GL_TEXTURE_2D, modelData.textureID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            int width, height, nrChannels;
            stbi_set_flip_vertically_on_load(true);
            unsigned char* data = stbi_load(modelData.texturePath.c_str(), &width, &height, &nrChannels, 0);
            if (data) {
                GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else {
                std::cerr << "Falha ao carregar textura: " << modelData.texturePath << std::endl;
            }
            stbi_image_free(data);
        }

        glBindVertexArray(0);
    }

    void ModelManager::BindShaderAttributes(GLuint shaderProgram) {
        glBindVertexArray(modelData.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, modelData.VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    void ModelManager::Render(const glm::vec3& position, const glm::vec3& orientation) {
        glBindVertexArray(modelData.VAO);
        if (modelData.textureID != 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, modelData.textureID);
        }
        glDrawArrays(GL_TRIANGLES, 0, modelData.vertexCount);
        glBindVertexArray(0);
    }
}

// Rest of the Balls class implementation remains the same, but using ModelManager
Balls::Balls() : shaderProgram(0) {}

Balls::~Balls() {
    // VAOs, VBOs and textures are now managed by ModelManager
}

void Balls::setup() {
    ShaderInfo shaders[] = {
        { GL_VERTEX_SHADER, "shaders/vertexShader.ver" },
        { GL_FRAGMENT_SHADER, "shaders/fragmentShader.frag" },
        { GL_NONE, NULL }
    };

    shaderProgram = LoadShaders(shaders);
    if (shaderProgram == 0) {
        std::cerr << "Erro ao criar shader program para Balls\n";
        return;
    }

    std::srand(12345);

    const float tableMinX = -0.7f;
    const float tableMaxX = 0.7f;
    const float tableMinZ = -1.3f;
    const float tableMaxZ = 1.3f;
    const float minDistance = 0.1f;

    std::vector<glm::vec3> positions;

    auto isPositionValid = [&](const glm::vec3& newPos) {
        for (const auto& pos : positions) {
            if (glm::distance(newPos, pos) < minDistance) {
                return false;
            }
        }
        return true;
        };

    for (int i = 0; i < 15; ++i) {
        glm::vec3 newPos;
        do {
            float x = tableMinX + static_cast<float>(std::rand()) /
                (static_cast<float>(RAND_MAX / (tableMaxX - tableMinX)));

            float z = tableMinZ + static_cast<float>(std::rand()) /
                (static_cast<float>(RAND_MAX / (tableMaxZ - tableMinZ)));

            newPos = glm::vec3(x, 0.05f, z);
        } while (!isPositionValid(newPos));

        positions.push_back(newPos);
    }

    for (int index = 0; index < 15; ++index) {
        Ball ball;
        std::string objPath = "PoolBalls/Ball" + std::to_string(index + 1) + ".obj";
        if (ball.model.Load(objPath)) {
            ball.model.Install();
            ball.model.BindShaderAttributes(shaderProgram);
            ball.position = positions[index];

            // Rotação fixa de 180° no X para correção + rotações aleatórias
            ball.rotation = glm::vec3(
                glm::radians(180.0f), // Fixa no X para orientação correta
                glm::radians(static_cast<float>(rand() % 360)), // Aleatório Y
                glm::radians(static_cast<float>(rand() % 360))  // Aleatório Z
            );

            balls.push_back(ball);
        }
    }
}

void Balls::render(const glm::mat4& view, const glm::mat4& projection,
    const glm::vec3& lightPos, const glm::vec3& viewPos,
    const glm::vec3& lightColor, bool useLighting) {
    glUseProgram(shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(viewPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));
    glUniform1i(glGetUniformLocation(shaderProgram, "useLighting"), useLighting ? 1 : 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);

    for (auto& ball : balls) {
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 2.0f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, ball.position);

        // Aplicar rotações
        model = glm::rotate(model, ball.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // X fixo
        model = glm::rotate(model, ball.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)); // Y aleatório
        model = glm::rotate(model, ball.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)); // Z aleatório
        model = glm::scale(model, glm::vec3(0.05f));

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        ball.model.Render(ball.position, glm::vec3(0.0f));
        glDisable(GL_POLYGON_OFFSET_FILL);
    }
}
