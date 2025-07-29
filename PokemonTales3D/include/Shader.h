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
#include "ResourceManager.h"

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
	void SetUniform(const std::string& name, glm::vec4 value) const;
	void SetUniform(const std::string& name, const GLfloat* matrix) const;

	unsigned int GetID();

private:
	unsigned int ID;
};

/* This resource manager doesn't unload functionality
Thus it can be used without Require and Release resources methods but only with GetShader method */
class ShaderManager : public ResourceManager<ShaderManager, Shader> {
public:
	ShaderManager(SharedContext* context);
	virtual ~ShaderManager();

	Shader* Load(const std::vector<std::string>* l_args);

	//Return false if the loading fails or the shader name already exists
	Shader* GetShader(const std::string& shaderName);

	void SetLightPos(glm::vec3 pos);
	void SetViewPos(glm::vec3 pos);

private:
	Shader* LoadShader(const char* vertexPath, const char* fragmentPath);

	glm::vec3 lightPos;
	glm::vec3 viewPos;
};