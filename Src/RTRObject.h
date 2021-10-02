//-----------------------------------------------------------------------------
// RTRObject.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include "RTRLighting.h"
#include "RTRShader.h"
#include "stb/stb_image.h"
#include <vector>

struct RTRPoint_t {
    float x, y, z, tex1, tex2;
};

struct RTRFace_t {
    unsigned int v0, v1, v2;
};

//-----------------------------------------------------------------------------
class RTRObject
{
public:
    RTRObject(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) {
        m_Position = position;
        m_Scale = scale;
        m_Rotation = rotation;
    }
    ~RTRObject() {}
    virtual void Init(std::string textureName, std::string textureName2);
    virtual void Render(RTRShader* shader);
    virtual void End();
    void SetMaterial(RTRMaterial_t mat) { m_Material = mat; }
    virtual const char* GetName() { return "RTRObject"; }
    virtual void SetPosition(glm::vec3 position) { m_Position = position; };
    virtual glm::vec3 GetPosition() { return m_Position; };
    virtual void SetScale(glm::vec3 scale) { m_Scale = scale; };
    virtual glm::vec3 GetScale() { return m_Scale; };
    virtual void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; };
    virtual glm::vec3 GetRotation() { return m_Rotation; };
    virtual void SetVelocity(float velocity) { m_Velocity = velocity; };
    virtual float GetVelocity() { return m_Velocity; };
    virtual unsigned int loadTexture(std::string textureFile);

public:
    unsigned int m_NumVertices{ 0 };
    unsigned int m_NumFaces{ 0 };
    RTRPoint_t* m_VertexPoints{ nullptr };
    RTRFace_t* m_Faces{ nullptr };
    RTRMaterial_t m_Material{ {0.19225, 0.19225, 0.19225 }, { 0.50754, 0.50754, 0.50754 }, { 0.508273, 0.508273, 0.508273 }, 128.0 };
    unsigned int m_VertexBuffer{ 0 };
    unsigned int m_VertexArray{ 0 };
    unsigned int m_FaceElementBuffer{ 0 };
    glm::vec3 m_Position{ 0, 0, 0 };
    glm::vec3 m_Scale{ 0, 0, 0 };
    glm::vec3 m_Rotation{ 0, 0, 0 };
    float m_Velocity{ 0 };
    unsigned int texture{ 0 };
    unsigned int texture2{ 0 };
};

//-----------------------------------------------------------------------------
class RTRCube : public RTRObject
{
public:
    RTRCube(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) : 
        RTRObject(position, scale, rotation) {}
    ~RTRCube() {}
    virtual void Init(std::string textureName, std::string textureName2);
    virtual const char* GetName() { return "RTRCube"; }
};

class RTRSphere : public RTRObject {
public:
    RTRSphere(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) : 
        RTRObject(position, scale, rotation) {}
    ~RTRSphere() {}
    virtual void Init(std::string textureName);
    virtual void Render(RTRShader* shader);
    virtual const char* GetName() { return "RTRSphere"; }
    virtual void InitSphere(std::vector<RTRPoint_t> vertices, std::vector<int> indices);
    virtual std::vector<RTRPoint_t> MakeSphereVertices(float radius, int stacks, int slices);
    virtual std::vector<int> MakeSphereIndex(int stacks, int slices);
};

class RTRCylinder : public RTRObject {
public:
    RTRCylinder(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) : 
        RTRObject(position, scale, rotation) {}
    ~RTRCylinder() {}
    virtual void Init(std::string textureName, std::string textureName2);
    virtual const char* GetName() { return "RTRCylinder"; }
};

class RTRPrism : public RTRObject {
public:
    RTRPrism(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) : 
        RTRObject(position, scale, rotation) {}
    ~RTRPrism() {}
    virtual void Init(std::string textureName, std::string textureName2);
    virtual const char* GetName() { return "RTRPrism"; }
};