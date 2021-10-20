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
        .Ambient = glm::vec3(0.5, 0.5, 0.5),
        .Diffuse = glm::vec3(1.0, 1.0, 1.0),
        .Specular = glm::vec3(0.8, 0.8, 0.8),
        .Direction = glm::vec3(1.0, 0.0, 0.0)
        });
    // Cornflower
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f),
        .Diffuse = glm::vec3(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(15.0f, 0.0, 0.0),
        .Constant = 0.5f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });
    // Navy
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(0.0, 0.0, 128.0f / 255.0f),
        .Diffuse = glm::vec3(0.0, 0.0, 128.0f / 255.0f),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(-15.0f, 0.0, 0.0),
        .Constant = 0.5f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });
    // Medium Sea Green
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(60.0f / 255.0f, 179.0f / 255.0f, 113.0f / 255.0f),
        .Diffuse = glm::vec3(60.0f / 255.0f, 179.0f / 255.0f, 113.0f / 255.0f),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(0.0, 5.0f, 0.0),
        .Constant = 0.5f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });
    // Lime Green
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(50.0f / 255.0f, 205.0f / 255.0f, 50.0f / 255.0f),
        .Diffuse = glm::vec3(50.0f / 255.0f, 205.0f / 255.0f, 50.0f / 255.0f),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(0.0, -5.0f, 0.0),
        .Constant = 0.5f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });
    // Firebrick
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(178.0f / 255.0f, 34.0f / 255.0f, 34.0f / 255.0f),
        .Diffuse = glm::vec3(178.0f / 255.0f, 34.0f / 255.0f, 34.0f / 255.0f),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(0.0, 0.0, 25.0f),
        .Constant = 0.5f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });
    // Maroon
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(128.0f / 255.0f, 0.0, 0.0),
        .Diffuse = glm::vec3(128.0f / 255.0f, 0.0, 0.0),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(0.0, 0.0, -25.0f),
        .Constant = 0.5f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });

    // Extra lights
    // Violet
    //m_LightingModel->AddLight({
    //    .Type = RTRLightType::PointLight,
    //    .Ambient = glm::vec3(238.0f / 255.0f, 130.0f / 255.0f, 238.0f / 255.0f),
    //    .Diffuse = glm::vec3(238.0f / 255.0f, 130.0f / 255.0f, 238.0f / 255.0f),
    //    .Specular = glm::vec3(1.0, 1.0, 1.0),
    //    .Position = glm::vec3(0.0, 0.0, 25.0f),
    //    .Constant = 0.5f,
    //    .Linear = 0.35f,
    //    .Quadratic = 0.44f
    //    });
    // Burlywood
    //m_LightingModel->AddLight({
    //    .Type = RTRLightType::PointLight,
    //    .Ambient = glm::vec3(222.0f / 255.0f, 184.0f / 255.0f, 135.0f / 255.0f),
    //    .Diffuse = glm::vec3(222.0f / 255.0f, 184.0f / 255.0f, 135.0f / 255.0f),
    //    .Specular = glm::vec3(1.0, 1.0, 1.0),
    //    .Position = glm::vec3(0.0, 0.0, -25.0f),
    //    .Constant = 0.5f,
    //    .Linear = 0.35f,
    //    .Quadratic = 0.44f
    //    });
    //m_LightingModel->AddLight({
    //    .Type = RTRLightType::PointLight,
    //    .Ambient = glm::vec3(255.0f / 255.0f, 255.0f / 255.0f, 0.0f),
    //    .Diffuse = glm::vec3(255.0f / 255.0f, 255.0f / 255.0f, 0.0f),
    //    .Specular = glm::vec3(1.0, 1.0, 1.0),
    //    .Position = glm::vec3(0.0, 0.0, 25.0f),
    //    .Constant = 0.5f,
    //    .Linear = 0.35f,
    //    .Quadratic = 0.44f
    //    });
}

void RTRWorld::StartObjects(glm::mat4 modelMatrix)
{
    // Load textures
    m_SmoothWoodTexture = LoadTexture("Src/Textures/SmoothWood/Wood066_1K_Color.jpg");
    m_DarkWoodTexture = LoadTexture("Src/Textures/DarkWood/Wood067_1K_Color.jpg");
    m_FloorWoodTexture = LoadTexture("Src/Textures/FloorWood/Wood071_1K_Color.jpg");
    m_ShinyMetalTexture = LoadTexture("Src/Textures/ShinyMetal/Metal012_1K_Color.jpg");
    m_RoughMetalTexture = LoadTexture("Src/Textures/RoughMetal/Metal010_1K_Color.jpg");
    m_OuterRimTexture = LoadTexture("Src/Textures/outer_container.png");

    // Test cube
    //RTRCube* TestCube = new RTRCube(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
    //    glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    //TestCube->SetName("TestCube");
    //TestCube->Init(m_SmoothWoodTexture, m_OuterRimTexture);

    //StaticPinballObjects.push_back(TestCube);

    // Static Pinball machine and related objects
    RTRCube* m_BottomPlane = new RTRCube(glm::vec3(0.0f, -3.0f, 0.0f), glm::vec3(14.0f, 0.5f, 20.0f), 
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    m_BottomPlane->SetName("m_BottomPlane");
    m_BottomPlane->Init(m_SmoothWoodTexture, m_RoughMetalTexture);

    StaticPinballObjects.push_back(m_BottomPlane);

    RTRCube* m_TopBar = new RTRCube(glm::vec3(0.0f, -0.13f, -19.5f), glm::vec3(14.0f, 2.0f, 0.5f),
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    m_TopBar->SetName("m_TopBar");
    m_TopBar->Init(m_DarkWoodTexture, m_OuterRimTexture);

    RTRCube* m_BottomBar = new RTRCube(glm::vec3(0.0f, -0.13f, 19.5f), glm::vec3(14.0f, 2.0f, 0.5f),
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    m_BottomBar->SetName("m_BottomBar");
    m_BottomBar->Init(m_DarkWoodTexture, m_OuterRimTexture);

    RTRCube* m_LeftBar = new RTRCube(glm::vec3(-13.5f, -0.13f, 0.0f), glm::vec3(0.5f, 2.0f, 19.0f),
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    m_LeftBar->SetName("m_LeftBar");
    m_LeftBar->Init(m_DarkWoodTexture, m_OuterRimTexture);

    RTRCube* m_RightBar = new RTRCube(glm::vec3(13.5f, -0.13f, 0.0f), glm::vec3(0.5f, 2.0f, 19.0f),
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    m_RightBar->SetName("m_RightBar");
    m_RightBar->Init(m_DarkWoodTexture, m_OuterRimTexture);

    RTRCube* m_TiltedBlock = new RTRCube(glm::vec3(18.65f, 0.0f, -3.0f), glm::vec3(0.5f, 1.0f, 2.0f),
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    m_TiltedBlock->DoRotation(glm::vec3(0.0f, 1.0f, 0.0f), 0.4f);
    RTRMaterial_t tiltedBlockMat = { {0.5, 0.5, 0.5 }, { 0.4, 0.4, 0.4 }, { 0.7, 0.7, 0.7 }, 64.0 };
    m_TiltedBlock->SetMaterial(tiltedBlockMat);
    m_TiltedBlock->SetName("m_TiltedBlock");
    m_TiltedBlock->Init(m_DarkWoodTexture, m_RoughMetalTexture);

    RTRCube* m_SideShootBar = new RTRCube(glm::vec3(10.0f, -0.13f, 0.18f), glm::vec3(0.5f, 2.0f, 14.0f),
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    m_SideShootBar->SetName("m_SideShootBar");
    m_SideShootBar->Init(m_DarkWoodTexture, m_OuterRimTexture);

    StaticCollidablePinballObjects.push_back(m_TopBar);
    StaticCollidablePinballObjects.push_back(m_BottomBar);
    StaticCollidablePinballObjects.push_back(m_LeftBar);
    StaticCollidablePinballObjects.push_back(m_RightBar);
    StaticCollidablePinballObjects.push_back(m_TiltedBlock);
    StaticCollidablePinballObjects.push_back(m_SideShootBar);

    // Dynamic pinball objects
    RTRCube* m_Plunger = new RTRCube(glm::vec3(11.75f, -1.5f, 3.2f), glm::vec3(0.5f, 0.5f, 3.0f),
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    RTRMaterial_t plungerMat = { {0.1, 0.8, 0.1 }, { 0.4, 0.4, 0.4 }, { 0.7, 0.7, 0.7 }, 64.0 };
    m_Plunger->SetMaterial(plungerMat);
    m_Plunger->SetName("m_Plunger");
    m_Plunger->Init(m_ShinyMetalTexture, m_RoughMetalTexture);

    DynamicPinballObjects.push_back(m_Plunger);

    // Test sphere
    MakeNewBall(modelMatrix);

    // Make 2D Uniform Grid
    // -5.0f
    MakeUniformGrid(-6.75f, -9.75f, 14, 20, modelMatrix);
}

void RTRWorld::MakeNewBall(glm::mat4 modelMatrix) {
    float scale = 0.5f;
    RTRSphere* newSphere = new RTRSphere(glm::vec3(11.75f, -1.5f, 14.0f), glm::vec3(scale, scale, scale),
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
    newSphere->SetRadius(scale);
    RTRMaterial_t sphereMat = { {0.1, 0.1, 0.1 }, { 0.5, 0.5, 0.5 }, { 0.8, 0.8, 0.8 }, 64.0 };
    newSphere->SetMaterial(sphereMat);
    newSphere->SetName("newSphere" + std::to_string(currBall));
    newSphere->Init(m_RoughMetalTexture, m_ShinyMetalTexture);
    DynamicObjects.push_back(newSphere);
}

unsigned int RTRWorld::LoadTexture(std::string textureFile)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char* data = stbi_load(textureFile.c_str(), &width, &height, &nrChannels, 0);

    std::string pngString = ".png";

    if (data)
    {
        if (strstr(textureFile.c_str(), pngString.c_str())) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture;
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
    int count = 0;

    for (glm::vec3 positions : UniformGridPositions) {
        RTRGrid* uniformGrid = new RTRGrid(positions, glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f), modelMatrix, DEFAULT_ANGLE);
        uniformGrid->SetMaterial(lineMat);
        uniformGrid->SetName("uniformGrid" + std::to_string(count));
        uniformGrid->Init();
        UniformGridObjects.push_back(uniformGrid);
    }
}

std::vector<glm::vec3> RTRWorld::MakeGridPositions(float xOffset, float zOffset, int horizontal, int vertical) {
    std::vector<glm::vec3> positions;

    for (int j = 0; j <= vertical; ++j) {
        for (int i = 0; i <= horizontal; ++i) {
            float x = ((float)i / (float)horizontal * (float)horizontal) + xOffset;
            float y = -1.0f;
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

    for (RTRObject* object : StaticCollidablePinballObjects) {
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
