#pragma once

#include "Shader.h"

class RectangleShape {
public:
	RectangleShape(Shader* l_shader);
	RectangleShape(glm::vec2 size, Shader* l_shader);

	void Draw();

	void SetSize(glm::vec2 l_size);
	void SetPos(glm::vec2 l_pos);
	void SetOrigin(glm::vec2 l_origin);
	void SetOrigin(Location location);
	void SetColor(glm::vec4 l_color);

	glm::vec2 GetSize() { return size; }

private:
	static unsigned int indices[6];

	void SetupGraphics();
	void ComputeVertices();

	Shader* shader;
	unsigned int VAO, VBO, EBO;
	glm::vec3 vertices[4];

	glm::vec2 size;
	glm::vec2 pos;
	glm::vec2 origin;
	glm::vec4 color;
};