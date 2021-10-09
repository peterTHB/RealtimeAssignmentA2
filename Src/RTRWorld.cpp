#include "RTRWorld.h"

RTRWorld::RTRWorld(glm::mat4 modelMatrix)
{
    StartLighting();
    StartObjects(modelMatrix);
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

void RTRWorld::StartObjects(glm::mat4 modelMatrix)
{
    // Test Cube
    //RTRObject* m_TestCube = new RTRCube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
    //    glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    //m_TestCube->Init("Src/Textures/wall.jpg", "Src/Textures/DarkWood/Wood067_1K_Color.png");

    //StaticPinballObjects.push_back(m_TestCube);

    // Static Pinball machine and related objects
    RTRCube* m_BottomPlane = new RTRCube(glm::vec3(0.0f, -3.0f, 0.0f), glm::vec3(7.0f, 0.25f, 10.0f), 
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    m_BottomPlane->Init("Src/Textures/wall.jpg", "Src/Textures/DarkWood/Wood067_1K_Color.png");

    RTRCube* m_TopBar = new RTRCube(glm::vec3(0.0f, -1.75f, -9.75f), glm::vec3(7.0f, 1.0f, 0.25f),
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    m_TopBar->Init("Src/Textures/wall.jpg", "Src/Textures/DarkWood/Wood067_1K_Color.png");

    RTRCube* m_BottomBar = new RTRCube(glm::vec3(0.0f, -1.75f, 9.75f), glm::vec3(7.0f, 1.0f, 0.25f),
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    m_BottomBar->Init("Src/Textures/wall.jpg", "Src/Textures/DarkWood/Wood067_1K_Color.png");

    RTRCube* m_LeftBar = new RTRCube(glm::vec3(-6.75f, -1.75f, 0.0f), glm::vec3(0.25f, 1.0f, 9.5f),
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    m_LeftBar->Init("Src/Textures/wall.jpg", "Src/Textures/DarkWood/Wood067_1K_Color.png");

    RTRCube* m_RightBar = new RTRCube(glm::vec3(6.75f, -1.75f, 0.0f), glm::vec3(0.25f, 1.0f, 9.5f),
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    m_RightBar->Init("Src/Textures/wall.jpg", "Src/Textures/DarkWood/Wood067_1K_Color.png");

    RTRCube* m_TiltedBlock = new RTRCube(glm::vec3(7.7f, -1.75f, -7.0f), glm::vec3(0.25f, 1.0f, 1.5f),
        glm::vec3(1.0f, 1.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    m_TiltedBlock->Init("Src/Textures/wall.jpg", "Src/Textures/DarkWood/Wood067_1K_Color.png");
    RTRMaterial_t tiltedBlockMat = { {0.1, 0.1, 0.8 }, { 0.4, 0.4, 0.4 }, { 0.7, 0.7, 0.7 }, 64.0 };
    m_TiltedBlock->SetMaterial(tiltedBlockMat);

    RTRCube* m_SideShootBar = new RTRCube(glm::vec3(5.5f, -1.75f, 2.0f), glm::vec3(0.25f, 1.0f, 7.5f),
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
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
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    m_Plunger->Init("Src/Textures/MetalStainless/Metal012_1K_Color.png", "Src/Textures/MetalRough/Metal014_1K_Color.png");
    RTRMaterial_t plungerMat = { {0.1, 0.8, 0.1 }, { 0.4, 0.4, 0.4 }, { 0.7, 0.7, 0.7 }, 64.0 };
    m_Plunger->SetMaterial(plungerMat);

    DynamicPinballObjects.push_back(m_Plunger);

    // Test sphere
    MakeNewBall(modelMatrix);

    // Make 2D Uniform Grid
    // -5.0f
    MakeUniformGrid(-6.75f, -9.75f, 14, 20, modelMatrix);

    //m_UniformGrid = new RTRGrid(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
    //    glm::vec3(0.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    //m_UniformGrid->Init();
    //RTRMaterial_t lineMat = { {1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, 64.0 };
    //m_UniformGrid->SetMaterial(lineMat);

    //RTRObject* newSphere = new RTRSphere(glm::vec3(0, 0, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f),
    //    glm::vec3(1.0f, 0.0f, 0.0f));
    //newSphere->Init();
    //RTRMaterial_t sphereMat = { {0.1, 0.1, 1.0 }, { 0.6, 0.6, 0.6 }, { 0.8, 0.8, 0.8 }, 64.0 };
    //newSphere->SetMaterial(sphereMat);
    //DynamicObjects.push_back(newSphere);
}

void RTRWorld::MakeNewBall(glm::mat4 modelMatrix) {
    RTRSphere* newSphere = new RTRSphere(glm::vec3(6.125f, -4.0f, 7.5f), glm::vec3(0.3f, 0.3f, 0.3f), 
        glm::vec3(0.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
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

void RTRWorld::MakeUniformGrid(float xOffset, float zOffset, int horizontal, int vertical, glm::mat4 modelMatrix) {
    UniformGridPositions = MakeGridPositions(xOffset, zOffset, horizontal, vertical);
    RTRMaterial_t lineMat = { {1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, 64.0 };

    for (glm::vec3 positions : UniformGridPositions) {
        RTRGrid* uniformGrid = new RTRGrid(positions, glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
        uniformGrid->Init();
        uniformGrid->SetMaterial(lineMat);

        UniformGridObjects.push_back(uniformGrid);
    }
}

std::vector<glm::vec3> RTRWorld::MakeGridPositions(float xOffset, float zOffset, int horizontal, int vertical) {
    std::vector<glm::vec3> positions;

    for (int j = 0; j <= vertical; ++j) {
        for (int i = 0; i <= horizontal; ++i) {
            float x = ((float)i / (float)horizontal * (float)horizontal) + xOffset;
            float y = -2.25f;
            float z = ((float)j / (float)vertical * (float)vertical) + zOffset;
            positions.push_back(glm::vec3(x, y, z));
        }
    }

    return positions;
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

    for (RTRObject* object : UniformGridObjects) {
        object->End();
    }

    StaticPinballObjects.clear();
    DynamicPinballObjects.clear();
    DynamicObjects.clear();
    UniformGridObjects.clear();
    UniformGridPositions.clear();
    skyboxFaces.clear();
    skyboxVertices.clear();

    cubemapTexture = 0;
    glDeleteVertexArrays(1, &skyboxVAO); skyboxVAO = 0;
    glDeleteBuffers(1, &skyboxVBO); skyboxVBO = 0;

    if (m_LightingModel != nullptr) { delete m_LightingModel; m_LightingModel = nullptr; }
}
