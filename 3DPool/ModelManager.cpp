#define STB_IMAGE_IMPLEMENTATION
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
#include "stb_image.h"
#include "ModelManager.h"

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

            // Posição
            modelData.vertices.push_back(v.x);
            modelData.vertices.push_back(v.y);
            modelData.vertices.push_back(v.z);

            // Normal
            glm::vec3 n = glm::normalize(v);
            modelData.vertices.push_back(n.x);
            modelData.vertices.push_back(n.y);
            modelData.vertices.push_back(n.z);

            // Coordenadas de textura
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
                stbi_set_flip_vertically_on_load(false); 

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
        // Calcular matriz de modelo
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::rotate(modelMatrix, orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrix = glm::scale(modelMatrix, modelData.scale);

        // Enviar matriz para o shader atual
        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        if (currentProgram != 0) {
            GLuint modelLoc = glGetUniformLocation(currentProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        }

        glBindVertexArray(modelData.VAO);
        if (modelData.textureID != 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, modelData.textureID);
        }
        glDrawArrays(GL_TRIANGLES, 0, modelData.vertexCount);
        glBindVertexArray(0);
    }
}