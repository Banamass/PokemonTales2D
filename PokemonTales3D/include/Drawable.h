#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SharedTypes.h"
#include "Model.h"

class Window;

struct Transform {
public:
	Transform();

	void Move(glm::vec3 move);
	void Scale(glm::vec3 scale);
	void Rotate(glm::vec3 rotation);

	void SetPosition(glm::vec3 pos);

	glm::mat4 GetTransform();

private:
	void ResetTransformations();
	void ComputeTransform();

	glm::vec3 translation;
	glm::vec3 scaling;
	glm::vec3 rotation; //pitch(around x) yaw(around y) roll(around z)
	glm::mat4 transform;

	friend class Window;
};

class Drawable {
public:
	struct Material {
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float shininess;
	};

	Drawable(Model* model, Shader* shader);
	Drawable(Model* model, Shader* shader, Material l_material);
	~Drawable();

	void Move(glm::vec3 move);
	void Scale(glm::vec3 scale);
	void Rotate(glm::vec3 rotation);

	void SetPosition(glm::vec3 pos);

	void SetMaterial(const Material& l_material);

	Transform* GetTransform() { return &transform; }

private:
	Material material;

	Model* model;
	Shader* shader;
	Transform transform;

	friend class Window;
};

class DrawableInstanced {
public:
	DrawableInstanced(Drawable* drawable, std::vector<Transform*>& instanceMatrix);

	unsigned int GetNbInstance();
	unsigned int GetInstanceVBO();
	bool IsVAOSetup();
	void VAOSetup();

private:
	//Reference drawable for multiple drawing
	Drawable* drawable;
	std::vector<Transform*> instanceTransforms;

	unsigned int instanceVBO;
	bool isVAOSetup;

	friend class Window;
};