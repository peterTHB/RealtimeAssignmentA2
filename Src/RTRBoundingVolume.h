#pragma once
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <sdl/SDL.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class RTRBoundingVolume {
public:
	RTRBoundingVolume();


private:

};

class RTRBV_2DCircle : public RTRBoundingVolume {
public:
	RTRBV_2DCircle() : RTRBoundingVolume() {}

private:
};