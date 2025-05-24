#include "background.h"
#include <iostream>
#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>

Background::Background() : shaderProgram(0), cubemapTexture(0) {}

Background::~Background() {
    if (cubemapTexture) glDeleteTextures(1, &cubemapTexture);
    if (shaderProgram) glDeleteProgram(shaderProgram);
}

bool Background::Load() {
    //Carregar texturas do cubemap
    cubemapTexture = loadCubemap();
    if (cubemapTexture == 0) return false;

    //Configurar geometria
    setupGeometry();

    //Carregar shaders (usando os mesmos shaders da mesa e bolas)
    ShaderInfo shaders[] = {
        { GL_VERTEX_SHADER, "shaders/vertexShader.ver" },
        { GL_FRAGMENT_SHADER, "shaders/fragmentShader.frag" },
        { GL_NONE, NULL }
    };

    shaderProgram = LoadShaders(shaders);
    if (shaderProgram == 0) return false;

    //Configurar atributos do shader
    model.Install();
    model.BindShaderAttributes(shaderProgram);

    return true;
}

GLuint Background::loadCubemap() {
    std::vector<std::string> faces = {
        "textures/skyboxRight.jpg",  
        "textures/skyboxLeft.jpg",   
        "textures/skyboxUp.jpg",    
        "textures/skyboxDown.jpg", 
        "textures/skyboxFront.jpg", 
        "textures/skyboxBack.jpg"    
    };

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    stbi_set_flip_vertically_on_load(true); // neste caso foi preciso inverter stb_image  

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
            glDeleteTextures(1, &textureID);
            return 0;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void Background::setupGeometry() {
    // Define os vértices da skybox 
    std::vector<float> vertices = {
        // Face direita
         1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

         // Face esquerda 
         -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

         // Face superior 
         -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
          1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
          1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
          1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

         // Face inferior 
         -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
          1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
          1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
          1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,

          // Face frontal 
          -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
          -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
           1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
           1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
           1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
          -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 

          // Face traseira 
          -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
          -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
           1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
           1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
           1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
          -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f
    };

    model.GetModelData().vertices = vertices;
    model.GetModelData().vertexCount = vertices.size() / 8; // 8 elementos por vértice
}

void Background::applySceneContext(const glm::mat4& view, const glm::mat4& projection,
    const glm::vec3& lightPos, const glm::vec3& viewPos,
    const glm::vec3& lightColor, bool useLighting) {

    if (!shaderProgram) return;

    //Cache dos uniform locations
    static GLint useSkyboxLoc = glGetUniformLocation(shaderProgram, "useSkybox");
    static GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    static GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    static GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    static GLint skyboxLoc = glGetUniformLocation(shaderProgram, "skybox");

    glUseProgram(shaderProgram);

    //Configuração do modo skybox
    glUniform1i(useSkyboxLoc, 1);

    //Configuração das matrizes
    const glm::mat4 modelMatrix(1.0f); // Matriz identidade
    const glm::mat4 viewSkybox = glm::mat4(glm::mat3(view)); // Remove translação

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewSkybox));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //Configuração da textura 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glUniform1i(skyboxLoc, 0);

    //Configurações de estado OpenGL 
    GLboolean cullFaceEnabled;
    glGetBooleanv(GL_CULL_FACE, &cullFaceEnabled);

    GLint depthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);

    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);

    //Renderização
    model.Render(glm::vec3(0.0f), glm::vec3(0.0f));

    //Restauração precisa do estado anterior
    if (cullFaceEnabled) glEnable(GL_CULL_FACE);
    glDepthFunc(depthFunc);
    glDepthMask(GL_TRUE);

    //Limpeza 
    glUniform1i(useSkyboxLoc, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}