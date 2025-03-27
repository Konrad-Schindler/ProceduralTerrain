#include "Shader.hpp"

static std::string shaderTypeToString(GLenum type) {
	switch (type) {
	case GL_VERTEX_SHADER: return "VERTEX";
	case GL_FRAGMENT_SHADER: return "FRAGMENT";
	case GL_TESS_CONTROL_SHADER: return "TESS_CONTROL";
	case GL_TESS_EVALUATION_SHADER: return "TESS_EVALUATION";
	case GL_GEOMETRY_SHADER: return "GEOMETRY";
	default: return "UNKNOWN";
	}
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::compile()
{
	ID = glCreateProgram();
	for (GLuint shader : shaders) {
		glAttachShader(ID, shader);
	}
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
}

void Shader::vertex(const char* path)
{
	addShader(path, GL_VERTEX_SHADER);
}
void Shader::fragment(const char* path)
{
	addShader(path, GL_FRAGMENT_SHADER);
}
void Shader::tese(const char* path)
{
	addShader(path, GL_TESS_EVALUATION_SHADER);
}
void Shader::tesc(const char* path)
{
	addShader(path, GL_TESS_CONTROL_SHADER);
}


bool Shader::checkCompileErrors(unsigned int shader, std::string type) {
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "ERROR::SHADER::COMPILING::" << type << ": " << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "ERROR::SHADER::LINKING: " << infoLog << std::endl;
		}
	}
	
	return !success;
}

void Shader::addShader(const char* path, GLenum shaderType)
{
	std::string code;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(path);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		code = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "ERROR::SHADER::" << shaderTypeToString(shaderType) << "::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* shaderCode = code.c_str();

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	if (!checkCompileErrors(shader, shaderTypeToString(shaderType)))
		shaders.push_back(shader);
}

void Shader::use() const
{
	assert(ID && "Shader not compiled");
	glUseProgram(ID);
}

void Shader::uniformMatrix4(const std::string& name, const glm::mat4& matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::uniformVector3(const std::string& name, const glm::vec3& vector) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector));
}

void Shader::uniformBool(const std::string& name, const bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::uniformInt(const std::string& name, const int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::uniformFloat(const std::string& name, const float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}