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

Balls::Balls(): shaderProgram(0)  {}

Balls::~Balls() {
    for (auto& ball : balls) {
        glDeleteVertexArrays(1, &ball.VAO);
        glDeleteBuffers(1, &ball.VBO);
        glDeleteTextures(1, &ball.textureID);
    }
}

void Balls::setup() {
    // Carregar shaders
    std::string vertexCode = readShaderSource("shaders/vertexShader.ver");
    std::string fragmentCode = readShaderSource("shaders/fragmentShader.frag");

    if (vertexCode.empty() || fragmentCode.empty()) {
        std::cerr << "Erro: Falha ao carregar shaders!" << std::endl;
        return;
    }


    shaderProgram = createShaderProgram(vertexCode.c_str(), fragmentCode.c_str());
    if (shaderProgram == 0) {
        std::cerr << "Erro: Falha ao criar programa de shader!" << std::endl;
        return;
    }

    std::srand(12345); // assim o aleatório é sempre o mesmo, mas mudamos no futuro se usarmos colliders 

    const float tableMinX = -0.7f;    // Margem esquerda
    const float tableMaxX = 0.7f;     // Margem direita
    const float tableMinZ = -1.3f;    // Fundo da mesa
    const float tableMaxZ = 1.3f;     // Frente da mesa
    const float minDistance = 0.1f;   // Distância mínima entre as bolas

    std::vector<glm::vec3> positions; // Declaração única

    // Função para verificar se a posição é válida
    auto isPositionValid = [&](const glm::vec3& newPos) {
        for (const auto& pos : positions) {
            if (glm::distance(newPos, pos) < minDistance) {
                return false;
            }
        }
        return true;
        };

    // Gera posições pseudo-aleatórias sem sobreposição
    for (int i = 0; i < 15; ++i) {
        glm::vec3 newPos;
        do {
            float x = tableMinX + static_cast<float>(std::rand()) /
                (static_cast<float>(RAND_MAX / (tableMaxX - tableMinX)));

            float z = tableMinZ + static_cast<float>(std::rand()) /
                (static_cast<float>(RAND_MAX / (tableMaxZ - tableMinZ)));

            newPos = glm::vec3(x, 0.05f, z); // Y fixo em 0.05
        } while (!isPositionValid(newPos)); // Gera novamente se a posição não for válida

        positions.push_back(newPos);
    }

    // Atribui as posições geradas às bolas
    for (int index = 0; index < 15; ++index) {
        Ball ball;
        std::string objPath = "PoolBalls/Ball" + std::to_string(index + 1) + ".obj";
        loadBallModel(objPath, ball);

        ball.position = positions[index];
        balls.push_back(ball);
    }

}

void Balls::render(const glm::mat4& view, const glm::mat4& projection,
    const glm::vec3& lightPos, const glm::vec3& viewPos,
    const glm::vec3& lightColor, bool useLighting) {
    glUseProgram(shaderProgram);

    // Configurações comuns
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(viewPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));
    glUniform1i(glGetUniformLocation(shaderProgram, "useLighting"), useLighting ? 1 : 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);

    for (auto& ball : balls) {
        // Ativa o glPolygonOffset para evitar z-fighting
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 2.0f); // Ajuste os valores conforme necessário

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, ball.position);
        model = glm::scale(model, glm::vec3(0.05f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ball.textureID);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture_diffuse"), 0);

        glBindVertexArray(ball.VAO);
        glDrawArrays(GL_TRIANGLES, 0, ball.vertexCount);
        glBindVertexArray(0);  // Reset do VAO
        glBindTexture(GL_TEXTURE_2D, 0); // Reset da textura

        // Desativa o glPolygonOffset após renderizar a bola
        glDisable(GL_POLYGON_OFFSET_FILL);
    }
}

void Balls::loadBallModel(const std::string& objPath, Ball& ball) {
    std::ifstream file(objPath);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir: " << objPath << std::endl;
        return;
    }

    std::vector<glm::vec3> temp_vertices;
    std::vector<unsigned int> indices;
    std::string mtlPath;
    std::string textureFile;

    // Extrair diretório base
    size_t lastSlash = objPath.find_last_of("\\/");
    std::string directory = (lastSlash != std::string::npos)
        ? objPath.substr(0, lastSlash + 1)
        : "";

    // Ler arquivo .obj
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "mtllib") {
            ss >> mtlPath;
            mtlPath = directory + mtlPath;
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

    // Carregar arquivo .mtl
    if (!mtlPath.empty()) {
        std::ifstream mtlFile(mtlPath);
        if (mtlFile.is_open()) {
            while (std::getline(mtlFile, line)) {
                std::istringstream ss(line);
                std::string tag;
                ss >> tag;
                if (tag == "map_Kd") {
                    std::string texFile;
                    ss >> texFile;
                    textureFile = directory + texFile;
                    break;
                }
            }
        }
        else {
            std::cerr << "Erro ao abrir .mtl: " << mtlPath << std::endl;
        }
    }

    // Processar geometria
    std::vector<float> data;
    for (unsigned int idx : indices) {
        const glm::vec3& v = temp_vertices[idx];

        // Posição
        data.push_back(v.x);
        data.push_back(v.y);
        data.push_back(v.z);

        // Normal (baseada na esfera)
        glm::vec3 n = glm::normalize(v);
        data.push_back(n.x);
        data.push_back(n.y);
        data.push_back(n.z);

        // Coordenadas UV esféricas
        float u = 0.5f + std::atan2(v.z, v.x) / (2 * glm::pi<float>());
        float v_tex = 0.5f - std::asin(v.y) / glm::pi<float>();
        data.push_back(u);
        data.push_back(v_tex);

    }

    // Configurar buffers OpenGL
    ball.vertexCount = static_cast<int>(indices.size());
    glGenVertexArrays(1, &ball.VAO);
    glGenBuffers(1, &ball.VBO);

    glBindVertexArray(ball.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, ball.VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    // Atributos dos vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // Carregar textura do .mtl
    if (!textureFile.empty()) {
        glGenTextures(1, &ball.textureID);
        glBindTexture(GL_TEXTURE_2D, ball.textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(textureFile.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cerr << "Falha ao carregar textura: " << textureFile << std::endl;
        }
        stbi_image_free(data);
    }
    else {
        std::cerr << "Nenhuma textura definida para: " << objPath << std::endl;
    }
}