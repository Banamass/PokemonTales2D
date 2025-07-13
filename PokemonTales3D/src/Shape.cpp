#include "Shape.h"

/*---------------RectangleShape---------------*/

unsigned int RectangleShape::indices[6] = { 0, 1, 3, 1, 2, 3 };

RectangleShape::RectangleShape(Shader* l_shader) : shader(l_shader) {
	size = glm::vec2(0, 0);
	pos = glm::vec2(0, 0);
	origin = glm::vec2(0, 0);
	color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	SetupGraphics();
}
RectangleShape::RectangleShape(glm::vec2 l_size, Shader* l_shader) : shader(l_shader) {
	size = l_size;
	pos = glm::vec2(0, 0);
	origin = glm::vec2(0, 0);
	color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	SetupGraphics();
}

void RectangleShape::Draw() {
	shader->use();
	shader->SetUniform("color", color);
	glm::mat4 projection = glm::ortho(0.0f, Constants::WIN_WIDTH, 0.0f, Constants::WIN_HEIGHT);
	shader->SetUniform("transform", glm::value_ptr(projection));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void RectangleShape::SetupGraphics() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	ComputeVertices();
}


void RectangleShape::ComputeVertices() {
	glm::vec2 realPos = pos - origin;
	vertices[0] = glm::vec3(realPos.x, realPos.y, 0.0f);
	vertices[1] = glm::vec3(realPos.x, realPos.y + size.y, 0.0f);
	vertices[2] = glm::vec3(realPos.x + size.x, realPos.y + size.y, 0.0f);
	vertices[3] = glm::vec3(realPos.x + size.x, realPos.y, 0.0f);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RectangleShape::SetOrigin(Location location) { SetOrigin(LocationToPosition(size, location)); }
void RectangleShape::SetSize(glm::vec2 l_size) { size = l_size; ComputeVertices(); }
void RectangleShape::SetPos(glm::vec2 l_pos) { pos = l_pos; ComputeVertices(); }
void RectangleShape::SetOrigin(glm::vec2 l_origin) { origin = l_origin; ComputeVertices(); }
void RectangleShape::SetColor(glm::vec4 l_color) { color = l_color; }