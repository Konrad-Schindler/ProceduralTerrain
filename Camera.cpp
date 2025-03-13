#include "Camera.hpp"

Camera::Camera() {}

glm::mat4 Camera::getProjection() const
{
	return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

glm::mat4 Camera::getView() const
{
	return glm::lookAt(position, position + (front * 10.f), up);
}

void Camera::translate(glm::vec3 translation)
{
	position += translation;
}

void Camera::rotate(float yaw, float pitch, float roll)
{
	glm::mat4 rotation(1.0f);
	glm::mat4 rolling = glm::rotate(rotation, roll, front);
	up = glm::normalize(glm::vec3(rolling * glm::vec4(up, 0.0f)));
	glm::mat4 pitching = glm::rotate(rotation, pitch, glm::cross(front, up));
	front = glm::normalize(glm::vec3(pitching * glm::vec4(front, 0.0f)));
	up = glm::normalize(glm::vec3(pitching * glm::vec4(up, 0.0f)));
	glm::mat4 yawing = glm::rotate(rotation, yaw, up);
	front = glm::normalize(glm::vec3(yawing * glm::vec4(front, 0.0f)));
}

void Camera::forward(float amount)
{
	position += front * amount;
}

void Camera::backward(float amount)
{
	position -= front * amount;
}

void Camera::left(float amount)
{
	position -= glm::normalize(glm::cross(front, up)) * amount;
}

void Camera::right(float amount)
{
	position += glm::normalize(glm::cross(front, up)) * amount;
}

void Camera::yaw(float amount)
{
	rotate(amount, 0.0f, 0.0f);
}

void Camera::pitch(float amount)
{
	rotate(0.0f, amount, 0.0f);
}

void Camera::roll(float amount)
{
	rotate(0.0f, 0.0f, amount);
}

void Camera::lookAt(glm::vec3 target)
{
	front = glm::normalize(target - position);
	glm::vec3 referenceVector = glm::vec3(0.0f, 1.0f, 0.0f);
	if (glm::cross(front, referenceVector) == glm::vec3(0.0f, 0.0f, 0.0f)) referenceVector = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 right = glm::normalize(glm::cross(front, referenceVector));
	up = glm::cross(right, front);
}
