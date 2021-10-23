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
void RTRObject::Init(unsigned int texture, unsigned int texture2)
{
    glGenBuffers(1, &m_VertexBuffer);
    glActiveTexture(GL_TEXTURE0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_NumVertices * sizeof(RTRPoint_t5), m_VertexPoints, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m_FaceElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumFaces * sizeof(RTRFace_t), m_Faces, GL_STATIC_DRAW);

    m_Texture = texture;
    m_Texture2 = texture2;
}

void RTRObject::Render(RTRShader* shader)
{
    shader->SetMaterial("u_ObjectMaterial", m_Material);
    shader->SetInt("u_ObjectMaterial.Diffuse", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    shader->SetInt("u_ObjectMaterial.Specular", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_Texture2);

    glUseProgram(shader->GetId());
    shader->SetInt("texture1", 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    shader->SetInt("texture2", 3);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_Texture2);

    glUseProgram(shader->GetId());
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

void RTRObject::DoRotation(glm::vec3 rotation, float angleRads)
{
    m_TransformMatrix = glm::rotate(m_TransformMatrix, angleRads, rotation);
}

void RTRObject::DoTranslation(glm::vec3 translation)
{
    m_TransformMatrix = glm::translate(m_TransformMatrix, translation);
}

void RTRObject::DoScale(glm::vec3 scale)
{
    m_TransformMatrix = glm::scale(m_TransformMatrix, scale);
}

//-----------------------------------------------------------------------------

void RTRCube::Init(unsigned int texture, unsigned int texture2)
{
    m_NumVertices = 36;
    m_NumFaces = 2;

    m_VertexPoints = new RTRPoint_t5[]{
        // Position TexCoords
        // Right
        { 0.5f, -0.5f, 0.5f, 1.0f, 0 }, { 0.5f, -0.5f, -0.5f, 0, 0 }, { 0.5f, 0.5f, -0.5f, 0, 1.0f },
        { 0.5f, -0.5f, 0.5f, 1.0f, 0 }, { 0.5f, 0.5f, -0.5f, 0, 1.0f }, { 0.5f, 0.5f, 0.5f, 1.0f, 1.0f },
        // Left
        { -0.5f, -0.5f, -0.5f, 1.0f, 0 }, { -0.5f, -0.5f, 0.5f, 0, 0 }, { -0.5f, 0.5f, 0.5f, 0, 1.0f },
        { -0.5f, -0.5f, -0.5f, 1.0f, 0 }, { -0.5f, 0.5f, 0.5f, 0, 1.0f }, { -0.5f, 0.5f, -0.5f, 1.0f, 1.0f },
        // Top
        { -0.5f, 0.5f, 0.5f, 1.0f, 0 }, { 0.5f, 0.5f, 0.5f, 0, 0  }, { 0.5f, 0.5f, -0.5f, 0, 1.0f },
        { -0.5f, 0.5f, 0.5f, 1.0f, 0 }, { 0.5f, 0.5f, -0.5f, 0, 1.0f }, { -0.5f, 0.5f, -0.5f, 1.0f, 1.0f },
        // Bottom
        { -0.5f, -0.5f, -0.5f, 1.0f, 0 }, { 0.5f, -0.5f, -0.5f, 0, 0 }, { 0.5f, -0.5f, 0.5f, 0, 1.0f },
        { -0.5f, -0.5f, -0.5f, 1.0f, 0 }, { 0.5f, -0.5f, 0.5f, 0, 1.0f }, { -0.5f, -0.5f, 0.5f, 1.0f, 1.0f },
        // Front
        { -0.5f, -0.5f, 0.5f, 1.0f, 0 }, { 0.5f, -0.5f, 0.5f, 0, 0  }, { 0.5f, 0.5f, 0.5f, 0, 1.0f },
        { -0.5f, -0.5f, 0.5f, 1.0f, 0 }, { 0.5f, 0.5f, 0.5f, 0, 1.0f  }, { -0.5f, 0.5f, 0.5f, 1.0f, 1.0f },
        // Back
        { 0.5f, -0.5f, -0.5f, 1.0f, 0 }, { -0.5f, -0.5f, -0.5f, 0, 0 }, { -0.5f, 0.5f, -0.5f, 0, 1.0f },
        { 0.5f, -0.5f, -0.5f, 1.0f, 0 }, { -0.5f, 0.5f, -0.5f, 0, 1.0f }, { 0.5f, 0.5f, -0.5f, 1.0f, 1.0f }
    };

    m_Faces = new RTRFace_t[]{
        { 0, 1, 2 }, {0, 2, 3}
    };

    m_Position = glm::vec3(m_TransformMatrix[3]);

    m_BoundingVolume = new RTRBV_AABB(m_Position, m_Scale, m_TransformMatrix);
    m_BoundingVolume->Init();

    RTRObject::Init(texture, texture2);
}

//-----------------------------------------------------------------------------

void RTRSphere::Init(unsigned int texture, unsigned int texture2)
{
    std::vector<RTRPoint_t5> allVertices = RTRSphere::MakeSphereVertices(24, 64);
    std::vector<int> allIndices = RTRSphere::MakeSphereIndex(24, 64);

    m_Position = glm::vec3(m_TransformMatrix[3]);

    m_BoundingVolume = new RTRBV_2DCircle(m_Position, m_Scale, m_TransformMatrix, 64, m_Radius);
    m_BoundingVolume->Init();

    RTRSphere::InitSphere(allVertices, allIndices);
    m_Texture = texture;
    m_Texture2 = texture2;
}

void RTRSphere::Render(RTRShader* shader)
{
    shader->SetMaterial("u_ObjectMaterial", m_Material);
    shader->SetInt("u_ObjectMaterial.Diffuse", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    shader->SetInt("u_ObjectMaterial.Specular", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_Texture2);

    glUseProgram(shader->GetId());
    shader->SetInt("texture1", 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    shader->SetInt("texture2", 3);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_Texture2);

    glBindVertexArray(m_VertexArray);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_NumVertices);
    glBindVertexArray(0);
}

void RTRSphere::InitSphere(std::vector<RTRPoint_t5> vertices, std::vector<int> indices) {
    glGenBuffers(1, &m_VertexBuffer);
    glActiveTexture(GL_TEXTURE0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(RTRPoint_t5), vertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_FaceElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
}

// Sphere code from Sphere section 
// http://isoptera.lcsc.edu/~seth/cs492/examples/modern-tutorials/basic_texturing/sphere.cpp
std::vector<RTRPoint_t5> RTRSphere::MakeSphereVertices(int stacks, int slices) {
    std::vector<RTRPoint_t5> vertices;

    int n = 2 * (slices + 1) * stacks;
    int i = 0;

    for (float theta = -glm::pi<float>() / 2; theta < glm::pi<float>() / 2 - 0.0001; theta += glm::pi<float>() / stacks) {
        for (float phi = -glm::pi<float>(); phi <= M_PI + 0.0001; phi += 2 * glm::pi<float>() / slices) {
            float s = (float)phi / slices;
            float t = (float)theta / stacks;

            RTRPoint_t5 points1 = RTRPoint_t5(cos(theta) * sin(phi), -sin(theta), cos(theta) * cos(phi), s, t);
            RTRPoint_t5 points2 = RTRPoint_t5(cos(theta + glm::pi<float>() / stacks) * sin(phi), -sin(theta + glm::pi<float>() / stacks), 
                cos(theta + glm::pi<float>() / stacks) * cos(phi), s, t);

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

void RTRSphere::End() {

}

//-----------------------------------------------------------------------------

void RTRPrism::Init(unsigned int texture, unsigned int texture2)
{
    m_NumVertices = 24;
    m_NumFaces = 2;

    m_VertexPoints = new RTRPoint_t5[]{
        // Position TexCoords
        // Left
        { -0.5f, -0.5f, -0.5f, 1.0f, 0 }, { -0.5f, -0.5f, 0.5f, 0, 0 }, { -0.5f, 0.5f, 0.5f, 0, 1.0f },
        { -0.5f, -0.5f, -0.5f, 1.0f, 0 }, { -0.5f, 0.5f, 0.5f, 0, 1.0f }, { -0.5f, 0.5f, -0.5f, 1.0f, 1.0f },
        // Top
        { -0.5f, 0.5f, 0.5f, 1.0f, 0 }, { 0.5f, 0.5f, -0.5f, 0, 1.0f }, { -0.5f, 0.5f, -0.5f, 1.0f, 1.0f },
        // Bottom
        { -0.5f, -0.5f, 0.5f, 0, 1.0f }, { -0.5f, -0.5f, -0.5f, 0, 0 }, { 0.5f, -0.5f, -0.5f, 1.0f, 0 },
        // Front
        { -0.5f, -0.5f, 0.5f, 0, 0 }, { 0.5f, -0.5f, -0.5f, 1.0f, 0 }, { 0.5f, 0.5f, -0.5f, 1.0f, 1.0f },
        { -0.5f, -0.5f, 0.5f, 0, 0 }, { 0.5f, 0.5f, -0.5f, 1.0f, 1.0f  }, { -0.5f, 0.5f, 0.5f, 0, 1.0f },
        // Back
        { 0.5f, -0.5f, -0.5f, 1.0f, 0 }, { -0.5f, -0.5f, -0.5f, 0, 0 }, { -0.5f, 0.5f, -0.5f, 0, 1.0f },
        { 0.5f, -0.5f, -0.5f, 1.0f, 0 }, { -0.5f, 0.5f, -0.5f, 0, 1.0f }, { 0.5f, 0.5f, -0.5f, 1.0f, 1.0f }
    };

    m_Faces = new RTRFace_t[]{
        { 0, 1, 2 }, {0, 2, 3}
    };

    m_Position = glm::vec3(m_TransformMatrix[3]);

    m_BoundingVolume = new RTRBV_AABB(m_Position, m_Scale, m_TransformMatrix);
    m_BoundingVolume->Init();

    RTRObject::Init(texture, texture2);
}

//-----------------------------------------------------------------------------

void RTRGrid::Init()
{
    m_NumVertices = 6;
    m_NumFaces = 2;

    m_Position = glm::vec3(m_TransformMatrix[3]);

    m_VertexPoints3 = new RTRPoint_t3[]{
        { -0.5f, 0, 0.5f }, { 0.5f, 0, 0.5f }, { 0.5f, 0, -0.5f },
        { -0.5f, 0, 0.5f }, { 0.5f, 0, -0.5f }, { -0.5f, 0, -0.5f }
    };

    m_Faces = new RTRFace_t[]{
        { 0, 1, 2 }, {0, 2, 3}
    };

    InitGrid();
}

void RTRGrid::InitGrid() 
{
    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_NumVertices * sizeof(RTRPoint_t3), m_VertexPoints3, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_FaceElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumFaces * sizeof(RTRFace_t), m_Faces, GL_STATIC_DRAW);
}

void RTRGrid::Render(RTRShader* shader) {
    shader->SetMaterial("u_ObjectMaterial", m_Material);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUseProgram(shader->GetId());
    glBindVertexArray(m_VertexArray);
    glDrawArrays(GL_TRIANGLES, 0, m_NumVertices);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RTRGrid::End()
{
    if (m_VertexPoints3 != nullptr) { delete m_VertexPoints3; m_VertexPoints3 = nullptr; }
    RTRObject::End();
}
