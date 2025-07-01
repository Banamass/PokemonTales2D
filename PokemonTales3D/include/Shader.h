#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SharedTypes.h"

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void use();
	void unuse();

	void SetUniform(const std::string& name, bool value) const;
	void SetUniform(const std::string& name, int value) const;
	void SetUniform(const std::string& name, float value) const;
	void SetUniform(const std::string& name, glm::vec3 value) const;
	void SetUniform(const std::string& name, const GLfloat* matrix) const;

	unsigned int GetID();

private:
	unsigned int ID;
};

class ShaderManager {
public:
	ShaderManager(SharedContext* context);
	~ShaderManager();

	//Return false if the loading fails or the shader name already exists
	bool LoadShader(const std::string& shaderName,
		const char* vertexPath, const char* fragmentPath);
	Shader* GetShader(const std::string& shaderName);

	void SetLightPos(glm::vec3 pos);
	void SetViewPos(glm::vec3 pos);

private:
	std::unordered_map<std::string, Shader> shaders;
};