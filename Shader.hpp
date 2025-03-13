#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const char* vertexPath);
	~Shader();
	void use() const;
	void uniformMatrix4(const std::string& name, const glm::mat4& matrix) const;
	void uniformVector3(const std::string& name, const glm::vec3& vector) const;
	void uniformBool(const std::string& name, const bool value) const;
	void uniformInt(const std::string& name, const int value) const;
	void uniformFloat(const std::string& name, const float value) const;
private:
	unsigned int ID;
	void checkCompileErrors(unsigned int shader, std::string type);
};