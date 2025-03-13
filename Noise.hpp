#pragma once

#include <glm/glm.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

enum Noise {
	Perlin
};

// https://iquilezles.org/articles/fbm/
float fractionalBrownianMotion(Noise noise, glm::vec2 position, int numOctaves, int gridSize);
