//-----------------------------------------------------------------------------
// RTRObject.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include "RTRLighting.h"
#include "RTRShader.h"
#include <vector>

struct RTRPoint_t {
    float x, y, z;
};

struct RTRFace_t {
    unsigned int v0, v1, v2;
};

//-----------------------------------------------------------------------------
class RTRObject
{
public:
    RTRObject() {}
    ~RTRObject() {}
    virtual void Init();
    virtual void Render(RTRShader* shader);
    virtual void End();
    void SetMaterial(RTRMaterial_t mat) { m_Material = mat; }
    virtual const char* GetName() { return "RTRObject"; }
    virtual void SetPosition(glm::vec3 position) { m_position = position; };
    virtual glm::vec3 GetPosition() { return m_position; };

public:
    unsigned int m_NumVertices{ 0 };
    unsigned int m_NumFaces{ 0 };
    RTRPoint_t* m_VertexPoints{ nullptr };
    RTRFace_t* m_Faces{ nullptr };
    RTRMaterial_t m_Material{ {0.19225, 0.19225, 0.19225 }, { 0.50754, 0.50754, 0.50754 }, { 0.508273, 0.508273, 0.508273 }, 128.0 };
    unsigned int m_VertexBuffer{ 0 };
    unsigned int m_VertexArray{ 0 };
    unsigned int m_FaceElementBuffer{ 0 };
    glm::vec3 m_position{ 0, 0, 0 };
};

//-----------------------------------------------------------------------------
class RTRCube : public RTRObject
{
public:
    RTRCube() : RTRObject() {}
    ~RTRCube() {}
    virtual void Init();
    virtual const char* GetName() { return "RTRCube"; }
};

class RTRSphere : public RTRObject {
public:
    RTRSphere() : RTRObject() {}
    ~RTRSphere() {}
    virtual void Init();
    virtual void Render(RTRShader* shader);
    virtual const char* GetName() { return "RTRSphere"; }
    virtual void InitSphere(std::vector<glm::vec3> vertices, std::vector<int> indices);
    virtual std::vector<glm::vec3> MakeSphereVertices(float radius, int stacks, int slices);
    virtual std::vector<int> MakeSphereIndex(int stacks, int slices);
};

class RTRCylinder : public RTRObject {
public:
    RTRCylinder() : RTRObject() {}
    ~RTRCylinder() {}
    virtual void Init();
    virtual const char* GetName() { return "RTRCylinder"; }
};

class RTRPrism : public RTRObject {
public:
    RTRPrism() : RTRObject() {}
    ~RTRPrism() {}
    virtual void Init();
    virtual const char* GetName() { return "RTRPrism"; }
};