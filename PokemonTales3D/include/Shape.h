#pragma once

#include "Shader.h"
#include "Drawable.h"

//This class is a drawable static which represents a rectangle
class RectangleShape : public DrawableStatic {
public:
	RectangleShape(Shader* l_shader);
	RectangleShape(glm::vec2 size, Shader* l_shader);
	virtual ~RectangleShape();

	virtual void Draw(glm::mat4& cameraMatrix);

	void SetSize(glm::vec2 l_size);
	void SetColor(glm::vec4 l_color);
	void SetZ(float l_z);
	glm::vec2 GetSize();

	virtual FloatRect GetFloatRect();

private:
	static unsigned int indices[6];

	void SetupGraphics();
	void ComputeVertices();

	Shader* shader;
	unsigned int VAO, VBO, EBO;
	glm::vec3 vertices[4];
	glm::vec2 size;

	float z;
	glm::vec4 color;
};