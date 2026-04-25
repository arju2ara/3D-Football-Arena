#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

#define PI 3.1416

using namespace std;

const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;

class Sphere
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    unsigned int sphereVAO;

    Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18,
        glm::vec3 amb = glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3 diff = glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3 spec = glm::vec3(0.5f, 0.5f, 0.5f),
        float shiny = 32.0f) : verticesStride(32)
    {
        set(radius, sectorCount, stackCount, amb, diff, spec, shiny);
        buildCoordinatesAndIndices();
        buildVertices();

        glGenVertexArrays(1, &sphereVAO);
        glBindVertexArray(sphereVAO);

        unsigned int sphereVBO;
        glGenBuffers(1, &sphereVBO);
        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        glBufferData(GL_ARRAY_BUFFER, this->getVertexSize(), this->getVertices(), GL_STATIC_DRAW);

        unsigned int sphereEBO;
        glGenBuffers(1, &sphereEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->getIndexSize(), this->getIndices(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        int stride = this->getVerticesStride();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 6));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    ~Sphere() {}

    void set(float radius, int sectors, int stacks, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        if (radius > 0)
            this->radius = radius;
        this->sectorCount = sectors;
        if (sectors < MIN_SECTOR_COUNT)
            this->sectorCount = MIN_SECTOR_COUNT;
        this->stackCount = stacks;
        if (stacks < MIN_STACK_COUNT)
            this->stackCount = MIN_STACK_COUNT;
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    unsigned int getVertexCount() const
    {
        return (unsigned int)coordinates.size() / 3;
    }

    unsigned int getVertexSize() const
    {
        return (unsigned int)vertices.size() * sizeof(float);
    }

    int getVerticesStride() const
    {
        return verticesStride;
    }

    const float* getVertices() const
    {
        return vertices.data();
    }

    unsigned int getIndexSize() const
    {
        return (unsigned int)indices.size() * sizeof(unsigned int);
    }

    const unsigned int* getIndices() const
    {
        return indices.data();
    }

    unsigned int getIndexCount() const
    {
        return (unsigned int)indices.size();
    }

private:
    void buildCoordinatesAndIndices()
    {
        float x, y, z, xz;
        float nx, ny, nz, lengthInv = 1.0f / radius;

        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle = PI / 2 - i * stackStep;
            xz = radius * cosf(stackAngle);
            y = radius * sinf(stackAngle);

            for (int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;

                x = xz * cosf(sectorAngle);
                z = xz * sinf(sectorAngle);
                coordinates.push_back(x);
                coordinates.push_back(y);
                coordinates.push_back(z);

                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                normals.push_back(nx);
                normals.push_back(ny);
                normals.push_back(nz);
            }
        }

        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);
            k2 = k1 + sectorCount + 1;

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                if (i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (stackCount - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
    }

    /*void buildVertices()
    {
        for (size_t i = 0; i < coordinates.size(); i += 3)
        {
            vertices.push_back(coordinates[i]);
            vertices.push_back(coordinates[i + 1]);
            vertices.push_back(coordinates[i + 2]);

            vertices.push_back(normals[i]);
            vertices.push_back(normals[i + 1]);
            vertices.push_back(normals[i + 2]);
        }
    }*/
    // sphere.h এ buildVertices() function এ texture coordinates add koro:

    void buildVertices()
    {
        for (size_t i = 0; i < coordinates.size(); i += 3)
        {
            // Position
            vertices.push_back(coordinates[i]);
            vertices.push_back(coordinates[i + 1]);
            vertices.push_back(coordinates[i + 2]);

            // Normal
            vertices.push_back(normals[i]);
            vertices.push_back(normals[i + 1]);
            vertices.push_back(normals[i + 2]);

            // ADD TEXTURE COORDINATES (এটা add koro)
            // Calculate UV from sphere coordinates
            float u = 0.5f + (atan2(normals[i + 2], normals[i]) / (2 * PI));
            float v = 0.5f - (asin(normals[i + 1]) / PI);
            vertices.push_back(u);
            vertices.push_back(v);
        }
    }

    float radius;
    int sectorCount;
    int stackCount;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<unsigned int> indices;
    std::vector<float> coordinates;
    int verticesStride;
};

#endif