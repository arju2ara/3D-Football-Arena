#ifndef cube_h
#define cube_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace std;

class Cube {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    Cube()
    {
        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    Cube(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    ~Cube()
    {
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteVertexArrays(1, &lightCubeVAO);
        glDeleteBuffers(1, &cubeVBO);
        glDeleteBuffers(1, &cubeEBO);
    }

    void drawCubeWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawCube(Shader& shader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        shader.use();

        shader.setVec3("color", glm::vec3(r, g, b));
        shader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void setMaterialisticProperty(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

private:
    unsigned int cubeVAO;
    unsigned int lightCubeVAO;
    unsigned int cubeVBO;
    unsigned int cubeEBO;

    void setUpCubeVertexDataAndConfigureVertexAttribute()
    {
        float cube_vertices[] = {
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

            1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

            0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

            0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
        };

        unsigned int cube_indices[] = {
            0, 3, 2,
            2, 1, 0,

            4, 5, 7,
            7, 6, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };

        glGenVertexArrays(1, &cubeVAO);
        glGenVertexArrays(1, &lightCubeVAO);
        glGenBuffers(1, &cubeVBO);
        glGenBuffers(1, &cubeEBO);

        glBindVertexArray(lightCubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
};

#endif