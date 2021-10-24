//-----------------------------------------------------------------------------
// RTRObject.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include "RTRLighting.h"
#include "RTRShader.h"
#include "RTRBoundingVolume.h"
#include "stb/stb_image.h"

//-----------------------------------------------------------------------------
class RTRObject
{
public:
    RTRObject(glm::mat4 modelMatrix, float angle) {
        m_TransformMatrix = modelMatrix;
        m_Angle = angle;
    }
    ~RTRObject() {}
    virtual void Init(unsigned int texture, unsigned int texture2);
    virtual void Render(RTRShader* shader);
    virtual void RenderEnvMapping(RTRShader* shader);
    virtual void End();
    void SetMaterial(RTRMaterial_t mat) { m_Material = mat; }
    virtual void SetName(std::string name) { m_ObjectName = name; };
    virtual std::string GetName() { return m_ObjectName; }
    virtual void SetPosition(glm::vec3 position) { m_Position = position; };
    virtual glm::vec3 GetPosition() { return m_Position; };
    virtual void SetTransformMatrix(glm::mat4 transformMatrix) { m_TransformMatrix = transformMatrix; };
    virtual glm::mat4 GetTransformMatrix() { return m_TransformMatrix; };
    virtual void SetTranslation(glm::vec3 translation) { m_Translation = translation; };
    virtual glm::vec3 GetTranslation() { return m_Translation; };
    virtual void SetScale(glm::vec3 scale) { m_Scale = scale; };
    virtual glm::vec3 GetScale() { return m_Scale; };
    virtual void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; };
    virtual glm::vec3 GetRotation() { return m_Rotation; };
    virtual void DoRotation(glm::vec3 rotation, float angleRads);
    virtual void DoTranslation(glm::vec3 translation);
    virtual void DoScale(glm::vec3 scale);
    virtual void SetAngle(float angle) { m_Angle = angle; };
    virtual float GetAngle() { return m_Angle; };
    virtual RTRBoundingVolume* GetBoundingVolume(){ return m_BoundingVolume; };
    virtual void SetTimer(float dt) { m_LightPosTimer = dt; };
    virtual float GetTimer() { return m_LightPosTimer; };

protected:
    unsigned int m_NumVertices{ 0 };
    unsigned int m_NumFaces{ 0 };
    RTRPoint_t5* m_VertexPoints{ nullptr };
    RTRFace_t* m_Faces{ nullptr };
    RTRMaterial_t m_Material{ {0.2f, 0.2f, 0.2f }, { 0.5f, 0.5f, 0.5f }, { 0.8f, 0.8f, 0.8f }, 64.0f };
    unsigned int m_VertexBuffer{ 0 };
    unsigned int m_VertexArray{ 0 };
    unsigned int m_FaceElementBuffer{ 0 };
    glm::mat4 m_TransformMatrix{ 0 };
    glm::vec3 m_Position{ 0 };
    glm::vec3 m_Translation{ 0 };
    glm::vec3 m_Scale{ 0 };
    glm::vec3 m_Rotation{ 0 };
    float m_Angle{ 0 };
    unsigned int m_Texture{ 0 };
    unsigned int m_Texture2{ 0 };
    std::string m_ObjectName{ "" };
    RTRBoundingVolume* m_BoundingVolume{ nullptr };
    float m_LightPosTimer{ 0 };
};

//-----------------------------------------------------------------------------
class RTRCube : public RTRObject
{
public:
    RTRCube(glm::mat4 modelMatrix, float angle) :
        RTRObject(modelMatrix, angle) {}
    ~RTRCube() {}
    virtual void Init(unsigned int texture, unsigned int texture2);
};

class RTRSphere : public RTRObject {
public:
    RTRSphere(glm::mat4 modelMatrix, float angle) :
        RTRObject(modelMatrix, angle) {}
    ~RTRSphere() {}
    virtual void Init(unsigned int texture, unsigned int texture2);
    virtual void Render(RTRShader* shader);
    virtual void End();
    virtual void InitSphere(std::vector<RTRPoint_t5> vertices, std::vector<int> indices);
    virtual std::vector<RTRPoint_t5> MakeSphereVertices(int stacks, int slices);
    virtual std::vector<int> MakeSphereIndex(int stacks, int slices);
    virtual void SetRadius(float radius) { m_Radius = radius; };
    virtual float GetRadius() { return m_Radius; };
    virtual void SetTotalDT(float dt) { m_TotalDT = dt; };
    virtual float GetTotalDT() { return m_TotalDT; };
    virtual void SetVerticalPower(float power) { m_VertPower = power; };
    virtual float GetVerticalPower() { return m_VertPower; };
    virtual void SetHorizontalPower(float power) { m_HoriPower = power; };
    virtual float GetHorizontalPower() { return m_HoriPower; };
    virtual void SetCanMove(bool move) { m_CanMove = move; };
    virtual bool GetCanMove() { return m_CanMove; };
    virtual void SetHasCollidedAABB(bool collided) { m_HasCollidedAABB = collided; };
    virtual bool GetHasCollidedAABB() { return m_HasCollidedAABB; };
    virtual void SetHasCollidedSphere(bool collided) { m_HasCollidedSphere = collided; };
    virtual bool GetHasCollidedSphere() { return m_HasCollidedSphere; };
    virtual void SetMovingForward(bool move) { m_MovingForward = move; };
    virtual bool GetMovingForward() { return m_MovingForward; };
    virtual void SetMovingRight(bool move) { m_MovingRight = move; };
    virtual bool GetMovingRight() { return m_MovingRight; };
    virtual void SetMovingLeft(bool move) { m_MovingLeft = move; };
    virtual bool GetMovingLeft() { return m_MovingLeft; };
    virtual void SetDidExit(bool exit) { m_DidExit = exit; };
    virtual bool GetDidExit() { return m_DidExit; };
    virtual void SetHitBottom(bool hit) { m_HitBottom = hit; };
    virtual bool GetHitBottom() { return m_HitBottom; };
    virtual void SetDestroyed(bool destroyed) { m_Destroyed = destroyed; };
    virtual bool GetDestroyed() { return m_Destroyed; };

private:
    float m_Radius{ 0 };
    float m_TotalDT{ 0 };
    float m_VertPower{ 0 };
    float m_HoriPower{ 0 };
    bool m_CanMove{ false };
    bool m_HasCollidedAABB{ false };
    bool m_HasCollidedSphere{ false };
    bool m_MovingForward{ false };
    bool m_MovingRight{ false };
    bool m_MovingLeft{ false };
    bool m_DidExit{ false };
    bool m_HitBottom{ false };
    bool m_Destroyed{ false };
};

class RTRPrism : public RTRObject {
public:
    RTRPrism(glm::mat4 modelMatrix, float angle) :
        RTRObject(modelMatrix, angle) {}
    ~RTRPrism() {}
    virtual void Init(unsigned int texture, unsigned int texture2);
};

class RTRGrid : public RTRObject
{
public:
    RTRGrid(glm::mat4 modelMatrix, float angle) :
        RTRObject(modelMatrix, angle) {}
    virtual void Init();
    virtual void InitGrid();
    virtual void Render(RTRShader* shader);
    virtual void End();

private:
    std::vector<glm::vec3> VerticesPoints;
    std::vector<glm::vec2> IndicesPoints;
    int m_Length{ 0 };
    RTRPoint_t3* m_VertexPoints3{ nullptr };
};