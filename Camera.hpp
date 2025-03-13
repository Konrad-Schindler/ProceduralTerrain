#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

struct Camera {
	Camera();

	glm::mat4 getProjection() const;
	glm::mat4 getView() const;

	void rotate(float yaw, float pitch, float roll);

	void forward(float amount);
	void backward(float amount);
	void left(float amount);
	void right(float amount);
	void yaw(float amount);
	void pitch(float amount);
	void roll(float amount);

	void lookAt(glm::vec3 target);

	double fov = 30.0;
	double aspectRatio = 16.0/9.0;
	double nearPlane = 0.1;
	double farPlane = 10.0;

	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 front = glm::vec3(1, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);

	void translate(glm::vec3 translation);
};