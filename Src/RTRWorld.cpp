#include "RTRWorld.h"

RTRWorld::RTRWorld()
{
    StartLighting();
    StartObjects();
    cubemapTexture = loadCubeMap(skyboxFaces);
    // Load Skybox
    LoadSkyboxVAO();
}

void RTRWorld::StartLighting()
{
    // Create and initialise lighting model
    m_LightingModel = new RTRLightingModel();

    // Add directional light to follow camera view direction
    m_LightingModel->AddLight({
        .Type = RTRLightType::DirectionalLight,
        .Ambient = glm::vec3(0.2, 0.2, 0.2),
        .Diffuse = glm::vec3(1.0, 1.0, 1.0),
        .Specular = glm::vec3(0.5, 0.5, 0.5),
        .Direction = glm::vec3(1.0, 0.0, 0.0)
        });
    // Add x-axis red spot lights (palce all at x+3.0 so they are positioned around shaded cube)
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(1.0, 0.0, 0.0),
        .Diffuse = glm::vec3(1.0, 0.0, 0.0),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(2.0 + 3.0, 0.0, 0.0),
        .Constant = 1.0f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(1.0, 0.0, 0.0),
        .Diffuse = glm::vec3(1.0, 0.0, 0.0),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(-2.0 + 3.0, 0.0, 0.0),
        .Constant = 1.0f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });
    // Add y-axis green spot lights
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(0.0, 0.2, 0.0),
        .Diffuse = glm::vec3(0.0, 1.0, 0.0),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(0.0 + 3.0, 2.0, 0.0),
        .Constant = 1.0f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(0.0, 0.2, 0.0),
        .Diffuse = glm::vec3(0.0, 1.0, 0.0),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(0.0 + 3.0, -2.0, 0.0),
        .Constant = 1.0f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });
    // Add z-axis blue spot lights
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(0.0, 0.0, 0.2),
        .Diffuse = glm::vec3(0.0, 0.0, 1.0),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(0.0 + 3.0, 0.0, 2.0),
        .Constant = 1.0f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(0.0, 0.0, 0.2),
        .Diffuse = glm::vec3(0.0, 0.0, 1.0),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(0.0 + 3.0, 0.0, -2.0),
        .Constant = 1.0f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });
}

void RTRWorld::StartObjects()
{
    // Test Cube
    //RTRObject* m_TestCube = new RTRCube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
    //    glm::vec3(1.0f, 0.0f, 0.0f));
    //m_TestCube->Init();

    //StaticPinballObjects.push_back(m_TestCube);

    // Static Pinball machine and related objects
    RTRCube* m_BottomPlane = new RTRCube(glm::vec3(0.0f, -3.0f, 0.0f), glm::vec3(7.0f, 0.25f, 10.0f), 
        glm::vec3(1.0f, 0.0f, 0.0f));
    m_BottomPlane->Init("Src/Textures/wall.jpg", "Src/Textures/DarkWood/Wood067_1K_Color.png");

    RTRCube* m_TopBar = new RTRCube(glm::vec3(0.0f, -1.75f, -9.75f), glm::vec3(7.0f, 1.0f, 0.25f),
        glm::vec3(1.0f, 0.0f, 0.0f));
    m_TopBar->Init("Src/Textures/wall.jpg", "Src/Textures/DarkWood/Wood067_1K_Color.png");

    RTRCube* m_BottomBar = new RTRCube(glm::vec3(0.0f, -1.75f, 9.75f), glm::vec3(7.0f, 1.0f, 0.25f),
        glm::vec3(1.0f, 0.0f, 0.0f));
    m_BottomBar->Init("Src/Textures/wall.jpg", "Src/Textures/DarkWood/Wood067_1K_Color.png");

    RTRCube* m_LeftBar = new RTRCube(glm::vec3(-6.75f, -1.75f, 0.0f), glm::vec3(0.25f, 1.0f, 9.5f),
        glm::vec3(1.0f, 0.0f, 0.0f));
    m_LeftBar->Init("Src/Textures/wall.jpg", "Src/Textures/DarkWood/Wood067_1K_Color.png");

    RTRCube* m_RightBar = new RTRCube(glm::vec3(6.75f, -1.75f, 0.0f), glm::vec3(0.25f, 1.0f, 9.5f),
        glm::vec3(1.0f, 0.0f, 0.0f));
    m_RightBar->Init("Src/Textures/wall.jpg", "Src/Textures/DarkWood/Wood067_1K_Color.png");

    RTRCube* m_TiltedBlock = new RTRCube(glm::vec3(7.7f, -1.75f, -7.0f), glm::vec3(0.25f, 1.0f, 1.5f),
        glm::vec3(1.0f, 1.0f, 0.0f));
    m_TiltedBlock->Init("Src/Textures/wall.jpg", "Src/Textures/DarkWood/Wood067_1K_Color.png");
    RTRMaterial_t tiltedBlockMat = { {0.1, 0.1, 0.8 }, { 0.4, 0.4, 0.4 }, { 0.7, 0.7, 0.7 }, 64.0 };
    m_TiltedBlock->SetMaterial(tiltedBlockMat);

    RTRCube* m_SideShootBar = new RTRCube(glm::vec3(5.5f, -1.75f, 2.0f), glm::vec3(0.25f, 1.0f, 7.5f),
        glm::vec3(1.0f, 0.0f, 0.0f));
    m_SideShootBar->Init("Src/Textures/wall.jpg", "Src/Textures/DarkWood/Wood067_1K_Color.png");

    StaticPinballObjects.push_back(m_BottomPlane);
    StaticPinballObjects.push_back(m_TopBar);
    StaticPinballObjects.push_back(m_BottomBar);
    StaticPinballObjects.push_back(m_LeftBar);
    StaticPinballObjects.push_back(m_RightBar);
    StaticPinballObjects.push_back(m_TiltedBlock);
    StaticPinballObjects.push_back(m_SideShootBar);

    //Dynamic pinball objects
    RTRCube* m_Plunger = new RTRCube(glm::vec3(6.125f, -2.5f, 10.4f), glm::vec3(0.25f, 0.25f, 1.5f),
        glm::vec3(1.0f, 0.0f, 0.0f));
    m_Plunger->Init("Src/Textures/MetalStainless/Metal012_1K_Color.png", "Src/Textures/MetalRough/Metal014_1K_Color.png");
    RTRMaterial_t plungerMat = { {0.1, 0.8, 0.1 }, { 0.4, 0.4, 0.4 }, { 0.7, 0.7, 0.7 }, 64.0 };
    m_Plunger->SetMaterial(plungerMat);

    DynamicPinballObjects.push_back(m_Plunger);

    // Test sphere
    MakeNewBall();

    //RTRObject* newSphere = new RTRSphere(glm::vec3(0, 0, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f),
    //    glm::vec3(1.0f, 0.0f, 0.0f));
    //newSphere->Init();
    //RTRMaterial_t sphereMat = { {0.1, 0.1, 1.0 }, { 0.6, 0.6, 0.6 }, { 0.8, 0.8, 0.8 }, 64.0 };
    //newSphere->SetMaterial(sphereMat);
    //DynamicObjects.push_back(newSphere);
}

void RTRWorld::MakeNewBall() {
    // Test sphere
    RTRSphere* newSphere = new RTRSphere(glm::vec3(6.125f, -2.5f, 8.5f), glm::vec3(0.3f, 0.3f, 0.3f), 
        glm::vec3(1.0f, 0.0f, 0.0f));
    newSphere->Init("Src/Textures/MetalRough/Metal014_1K_Color.png");
    RTRMaterial_t sphereMat = { {0.1, 0.1, 0.8 }, { 0.5, 0.5, 0.5 }, { 0.8, 0.8, 0.8 }, 64.0 };
    newSphere->SetMaterial(sphereMat);
    DynamicObjects.push_back(newSphere);
}

void RTRWorld::LoadSkyboxVAO() {
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * 4, skyboxVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

// from learnopengl
// Loading skybox
unsigned int RTRWorld::loadCubeMap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void RTRWorld::DrawSkybox() {
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

void RTRWorld::Done() {
    for (RTRObject* object : StaticPinballObjects) {
        object->End();
    }

    for (RTRObject* object : DynamicPinballObjects) {
        object->End();
    }

    for (RTRObject* object : DynamicObjects) {
        object->End();
    }

    StaticPinballObjects.clear();
    DynamicPinballObjects.clear();
    DynamicObjects.clear();
    skyboxFaces.clear();
    skyboxVertices.clear();

    cubemapTexture = 0;
    glDeleteVertexArrays(1, &skyboxVAO); skyboxVAO = 0;
    glDeleteBuffers(1, &skyboxVBO); skyboxVBO = 0;
}
