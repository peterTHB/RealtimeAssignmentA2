//-----------------------------------------------------------------------------
// RTRObject.cpp
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#include <iostream>
#include "RTRObject.h"

// README / TODO - This is just a simple start.
// * For your assignment, think carefully about how Shaders, Materials and Objects will hang together.
// * Have a hierarchy of differnet object types (static object, moving object, hierarchical object, object with force, etc...
// * Don't worry too much about optimising the buffer handling by, for example, merging all objects into a single 
// vertex buffer. Or sorting object based on shader or material to minimize switches. Spend your time on more imprtant 
// optimisation techniques that will add marks - for example your uniform grid implementation. 
// * For this assignment you can hardcode the vertex buffers for the differnet shapes (see cube) and then 
// scale, place and animate them with transformation matrices.

//-----------------------------------------------------------------------------
void RTRObject::Init()
{
    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_NumVertices * sizeof(RTRPoint_t), m_VertexPoints, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_FaceElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumFaces * sizeof(RTRFace_t), m_Faces, GL_STATIC_DRAW);
}

void RTRObject::Render(RTRShader *shader)
{
    shader->SetMaterial("u_ObjectMaterial", m_Material);
    glBindVertexArray(m_VertexArray);
    glDrawArrays(GL_TRIANGLES, 0, m_NumVertices);
    glBindVertexArray(0);
}

void RTRObject::End()
{
    glDeleteVertexArrays(1, &m_VertexArray); m_VertexArray = 0;
    glDeleteBuffers(1, &m_VertexBuffer); m_VertexBuffer = 0;
    glDeleteBuffers(1, &m_FaceElementBuffer); m_FaceElementBuffer = 0;
    if (m_VertexPoints != nullptr) { delete m_VertexPoints; m_VertexPoints = nullptr; }
    if (m_Faces != nullptr) { delete m_Faces; m_Faces = nullptr; }
}

//-----------------------------------------------------------------------------

void RTRCube::Init()
{
    m_NumVertices = 36;
    m_NumFaces = 2;

    m_VertexPoints = new RTRPoint_t[]{
        // Right
        { 1.0f, -1.0f, 1.0f }, { 1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, -1.0f },
        { 1.0f, 1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, -1.0f },
        // Left
        { -1.0f, -1.0f, -1.0f }, { -1.0f, -1.0f, 1.0f }, { -1.0f, 1.0f, 1.0f },
        { -1.0f, 1.0f, -1.0f }, { -1.0f, -1.0f, -1.0f }, { -1.0f, 1.0f, 1.0f },
        // Top
        { -1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, -1.0f },
        { -1.0f, 1.0f, -1.0f }, { -1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, -1.0f },
        // Bottom
        { -1.0f, -1.0f, -1.0f }, { 1.0f, -1.0f, -1.0f }, { 1.0f, -1.0f, 1.0f },
        { -1.0f, -1.0f, 1.0f }, { -1.0f, -1.0f, -1.0f }, { 1.0f, -1.0f, 1.0f },
        // Front
        { -1.0f, -1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f },
        { -1.0f, 1.0f, 1.0f }, { -1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f },
        // Back
        { 1.0f, -1.0f, -1.0f }, { -1.0f, -1.0f, -1.0f }, { -1.0f, 1.0f, -1.0f },
        { 1.0f, 1.0f, -1.0f }, { 1.0f, -1.0f, -1.0f }, { -1.0f, 1.0f, -1.0f }
    };

    m_Faces = new RTRFace_t[]{
        { 0, 1, 2 }, {0, 2, 3}
    };

    RTRObject::Init();
}

//-----------------------------------------------------------------------------

void RTRSphere::Init()
{
    std::vector<glm::vec3> allVertices = RTRSphere::MakeSphereVertices(1.0f, 24, 64);
    std::vector<int> allIndices = RTRSphere::MakeSphereIndex(24, 64);
    RTRSphere::InitSphere(allVertices, allIndices);
}

void RTRSphere::Render(RTRShader* shader)
{
    shader->SetMaterial("u_ObjectMaterial", m_Material);
    glBindVertexArray(m_VertexArray);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_NumVertices);
    glBindVertexArray(0);
}

void RTRSphere::InitSphere(std::vector<glm::vec3> vertices, std::vector<int> indices) {
    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_FaceElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
}

// Sphere code from Sphere section 
// http://isoptera.lcsc.edu/~seth/cs492/examples/modern-tutorials/basic_texturing/sphere.cpp
std::vector<glm::vec3> RTRSphere::MakeSphereVertices(float radius, int stacks, int slices) {
    std::vector<glm::vec3> vertices;

    int n = 2 * (slices + 1) * stacks;
    int i = 0;

    for (float theta = -glm::pi<float>() / 2; theta < glm::pi<float>() / 2 - 0.0001; theta += glm::pi<float>() / stacks) {
        for (float phi = -glm::pi<float>(); phi <= M_PI + 0.0001; phi += 2 * glm::pi<float>() / slices) {
            glm::vec3 points1 = glm::vec3(cos(theta) * sin(phi), -sin(theta), cos(theta) * cos(phi));
            glm::vec3 points2 = glm::vec3(cos(theta + glm::pi<float>() / stacks) * sin(phi), -sin(theta + glm::pi<float>() / stacks), cos(theta + glm::pi<float>() / stacks) * cos(phi));

            vertices.push_back(points1);
            vertices.push_back(points2);
        }
    }

    m_NumVertices = vertices.size();

    return vertices;
}

// Sphere indices creation
// https://stackoverflow.com/questions/26116923/modern-opengl-draw-a-sphere-and-cylinder
std::vector<int> RTRSphere::MakeSphereIndex(int stacks, int slices) {
    std::vector<int> indices;

    for (int i = 0; i < slices * stacks + slices; ++i) {

        indices.push_back(i);
        indices.push_back(i + slices + 1);
        indices.push_back(i + slices);

        indices.push_back(i + slices + 1);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    return indices;
}

//-----------------------------------------------------------------------------

void RTRCylinder::Init()
{
    RTRObject::Init();
}

//-----------------------------------------------------------------------------

void RTRPrism::Init()
{
    RTRObject::Init();
}
