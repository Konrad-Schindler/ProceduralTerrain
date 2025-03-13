#include "Noise.hpp"

typedef float (*noiseFunctionPtr)(glm::vec2);

float hash(int x, int y) {
    uint32_t h = (uint32_t)x * 73856093u ^ (uint32_t)y * 19349663u;
    h ^= (h >> 16);
    h *= 0x85ebca6bu;
    h ^= (h >> 13);
    h *= 0xc2b2ae35u;
    h ^= (h >> 16);
    return h;
}

float smoothstep(float t) {
    return t * t * (3 - 2 * t);
}

float perlinNoise(glm::vec2 position) {
    int x1 = position.x;
    int x2 = x1 + 1;
    int y1 = position.y;
    int y2 = y1 + 1;

    glm::vec2 topLeft = glm::vec2(std::cos(hash(x1, y1)), std::sin(hash(x1, y1)));
    glm::vec2 topRight = glm::vec2(std::cos(hash(x2, y1)), std::sin(hash(x2, y1)));
    glm::vec2 bottomLeft = glm::vec2(std::cos(hash(x1, y2)), std::sin(hash(x1, y2)));
    glm::vec2 bottomRight = glm::vec2(std::cos(hash(x2, y2)), std::sin(hash(x2, y2)));

    float x1x = position.x - x1;
    float x2x = position.x - x2;
    float y1y = position.y - y1;
    float y2y = position.y - y2;
    if (x1x == 0.0) x1x = 1e-8;
    if (y1y == 0.0) y1y = 1e-8;

    float topLeftValue = glm::dot(topLeft, glm::vec2(x1x, y1y));
    float topRightValue = glm::dot(topRight, glm::vec2(x2x, y1y));
    float bottomLeftValue = glm::dot(bottomLeft, glm::vec2(x1x, y2y));
    float bottomRightValue = glm::dot(bottomRight, glm::vec2(x2x, y2y));

    float tx = smoothstep(x1x);
    float ty = smoothstep(y1y);
    float topValue = glm::mix(topLeftValue, topRightValue, tx);
    float bottomValue = glm::mix(bottomLeftValue, bottomRightValue, tx);
    return glm::mix(topValue, bottomValue, ty);
}



static noiseFunctionPtr noiseFunction(Noise noise) {
    switch (noise) {
    case Noise::Perlin:
        return perlinNoise;
    }
}

// https://iquilezles.org/articles/fbm/
float fractionalBrownianMotion(Noise noise, glm::vec2 position, int numOctaves, int gridSize)
{
    float G = 0.5;  // or make it variable with argument H and G = H^-2
    float f = 1.0;
    float a = 1.0;
    float t = 0.0;
    position /= gridSize;
    noiseFunctionPtr fNoise = noiseFunction(noise);
    for (int i = 0; i < numOctaves; i++)
    {
        t += a * fNoise(f * position);
        f *= 2.0;
        a *= G;
    }
    return t;
}