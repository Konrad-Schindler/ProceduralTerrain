#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Shader {
public:
	~Shader();
	void compile();
	void vertex(const char* path);
	void fragment(const char* path);
	void tesc(const char* path);
	void tese(const char* path);
	void use() const;
	void uniformMatrix4(const std::string& name, const glm::mat4& matrix) const;
	void uniformVector3(const std::string& name, const glm::vec3& vector) const;
	void uniformBool(const std::string& name, const bool value) const;
	void uniformInt(const std::string& name, const int value) const;
	void uniformFloat(const std::string& name, const float value) const;
private:
	GLuint ID;
	std::vector<GLuint> shaders;
	bool checkCompileErrors(unsigned int shader, std::string type);
	void addShader(const char* path, GLenum shaderType);
};