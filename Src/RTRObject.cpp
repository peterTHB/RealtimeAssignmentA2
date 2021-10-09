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
void RTRObject::Init(std::string textureName, std::string textureName2)
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

    texture = LoadTexture(textureName);
    texture2 = LoadTexture(textureName2);

    /*texture = loadTexture("Src/Textures/wall.jpg");
    texture2 = loadTexture("Src/Textures/DarkWood/Wood067_1K_Color.png");*/
    //texture2 = loadTexture("Src/Textures/MetalStainless/Metal012_1K_Color.png");
    //texture2 = loadTexture("Src/Textures/MetalRough/Metal014_1K_Color.png");
}

void RTRObject::Render(RTRShader* shader)
{
    shader->SetMaterial("u_ObjectMaterial", m_Material);

    glUseProgram(shader->GetId());
    shader->SetInt("texture1", 0);
    shader->SetInt("texture2", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

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

unsigned int RTRObject::LoadTexture(std::string textureFile)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char* data = stbi_load(textureFile.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture;
}

//-----------------------------------------------------------------------------

void RTRCube::Init(std::string textureName, std::string textureName2)
{
    m_NumVertices = 36;
    m_NumFaces = 2;

    m_VertexPoints = new RTRPoint_t5[]{
        // Position TexCoords
        // Right
        { 1.0f, -1.0f, 1.0f, 1.0f, 0 }, { 1.0f, -1.0f, -1.0f, 0, 0 }, { 1.0f, 1.0f, -1.0f, 0, 1.0f },
        { 1.0f, -1.0f, 1.0f, 1.0f, 0 }, { 1.0f, 1.0f, -1.0f, 0, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        // Left
        { -1.0f, -1.0f, -1.0f, 1.0f, 0 }, { -1.0f, -1.0f, 1.0f, 0, 0 }, { -1.0f, 1.0f, 1.0f, 0, 1.0f },
        { -1.0f, -1.0f, -1.0f, 1.0f, 0 }, { -1.0f, 1.0f, 1.0f, 0, 1.0f }, { -1.0f, 1.0f, -1.0f, 1.0f, 1.0f },
        // Top
        { -1.0f, 1.0f, 1.0f, 1.0f, 0 }, { 1.0f, 1.0f, 1.0f, 0, 0  }, { 1.0f, 1.0f, -1.0f, 0, 1.0f },
        { -1.0f, 1.0f, 1.0f, 1.0f, 0 }, { 1.0f, 1.0f, -1.0f, 0, 1.0f }, { -1.0f, 1.0f, -1.0f, 1.0f, 1.0f },
        // Bottom
        { -1.0f, -1.0f, -1.0f, 1.0f, 0 }, { 1.0f, -1.0f, -1.0f, 0, 0 }, { 1.0f, -1.0f, 1.0f, 0, 1.0f },
        { -1.0f, -1.0f, -1.0f, 1.0f, 0 }, { 1.0f, -1.0f, 1.0f, 0, 1.0f }, { -1.0f, -1.0f, 1.0f, 1.0f, 1.0f },
        // Front
        { -1.0f, -1.0f, 1.0f, 1.0f, 0 }, { 1.0f, -1.0f, 1.0f, 0, 0  }, { 1.0f, 1.0f, 1.0f, 0, 1.0f },
        { -1.0f, -1.0f, 1.0f, 1.0f, 0 }, { 1.0f, 1.0f, 1.0f, 0, 1.0f  }, { -1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        // Back
        { 1.0f, -1.0f, -1.0f, 1.0f, 0 }, { -1.0f, -1.0f, -1.0f, 0, 0 }, { -1.0f, 1.0f, -1.0f, 0, 1.0f },
        { 1.0f, -1.0f, -1.0f, 1.0f, 0 }, { -1.0f, 1.0f, -1.0f, 0, 1.0f }, { 1.0f, 1.0f, -1.0f, 1.0f, 1.0f }
    };

    m_Faces = new RTRFace_t[]{
        { 0, 1, 2 }, {0, 2, 3}
    };

    if (m_Rotation != glm::vec3(0)) {
        m_TransformMatrix = glm::rotate(m_TransformMatrix, m_Angle, m_Rotation);
    }
    m_TransformMatrix = glm::translate(m_TransformMatrix, m_Translation);
    m_TransformMatrix = glm::scale(m_TransformMatrix, m_Scale);
    m_Position = glm::vec3(m_TransformMatrix[3]);

    RTRObject::Init(textureName, textureName2);
}

//-----------------------------------------------------------------------------

void RTRSphere::Init(std::string textureName)
{
    std::vector<RTRPoint_t5> allVertices = RTRSphere::MakeSphereVertices(1.0f, 24, 64);
    std::vector<int> allIndices = RTRSphere::MakeSphereIndex(24, 64);
    RTRSphere::InitSphere(allVertices, allIndices);

    if (m_Rotation != glm::vec3(0)) {
        m_TransformMatrix = glm::rotate(m_TransformMatrix, m_Angle, m_Rotation);
    }
    m_TransformMatrix = glm::translate(m_TransformMatrix, m_Translation);
    m_TransformMatrix = glm::scale(m_TransformMatrix, m_Scale);
    m_Position = glm::vec3(m_TransformMatrix[3]);

    texture = LoadTexture(textureName);
}

void RTRSphere::Render(RTRShader* shader)
{
    shader->SetMaterial("u_ObjectMaterial", m_Material);

    glUseProgram(shader->GetId());
    shader->SetInt("texture1", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

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
std::vector<RTRPoint_t5> RTRSphere::MakeSphereVertices(float radius, int stacks, int slices) {
    std::vector<RTRPoint_t5> vertices;

    int n = 2 * (slices + 1) * stacks;
    int i = 0;

    for (float theta = -glm::pi<float>() / 2; theta < glm::pi<float>() / 2 - 0.0001; theta += glm::pi<float>() / stacks) {
        for (float phi = -glm::pi<float>(); phi <= M_PI + 0.0001; phi += 2 * glm::pi<float>() / slices) {
            float s = (float)phi / slices;
            float t = (float)theta / stacks;

            RTRPoint_t5 points1 = RTRPoint_t5(cos(theta) * sin(phi), -sin(theta), cos(theta) * cos(phi), s, t);
            RTRPoint_t5 points2 = RTRPoint_t5(cos(theta + glm::pi<float>() / stacks) * sin(phi), -sin(theta + glm::pi<float>() / stacks), cos(theta + glm::pi<float>() / stacks) * cos(phi), s, t);

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

void RTRCylinder::Init(std::string textureName, std::string textureName2)
{
    RTRObject::Init(textureName, textureName2);
}

//-----------------------------------------------------------------------------

void RTRPrism::Init(std::string textureName, std::string textureName2)
{
    RTRObject::Init(textureName, textureName2);
}

//-----------------------------------------------------------------------------

void RTRGrid::Init()
{
    m_NumVertices = 6;
    m_NumFaces = 2;

    if (m_Rotation != glm::vec3(0)) {
        m_TransformMatrix = glm::rotate(m_TransformMatrix, m_Angle, m_Rotation);
    }
    m_TransformMatrix = glm::translate(m_TransformMatrix, m_Translation);
    m_TransformMatrix = glm::scale(m_TransformMatrix, m_Scale);
    m_Position = glm::vec3(m_TransformMatrix[3]);

    m_VertexPoints3 = new RTRPoint_t3[]{
        // Top
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
