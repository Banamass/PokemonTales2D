#pragma once

#include "Shader.h"
#include "Drawable.h"

class Shape {
public:
	Shape() : pos(0.0f, 0.0f){}
	virtual ~Shape(){}

	virtual void Draw() = 0;

	virtual void SetPos(glm::vec2 l_pos) { pos = l_pos; }
	virtual glm::vec2 GetPos() { return pos; }

protected:
	glm::vec2 pos;
};

class RectangleShape : public DrawableStatic {
public:
	RectangleShape(Shader* l_shader);
	RectangleShape(glm::vec2 size, Shader* l_shader);
	virtual ~RectangleShape();

	virtual void Draw(glm::mat4& cameraMatrix);

	void SetSize(glm::vec2 l_size);
	virtual void SetPos(glm::vec2 l_pos);
	virtual void SetOffset(glm::vec2 l_offset);
	void SetOrigin(glm::vec2 l_origin);
	void SetOrigin(Location location);
	void SetColor(glm::vec4 l_color);
	void SetZ(float l_z);

	glm::vec2 GetSize() { return size; }

private:
	static unsigned int indices[6];

	void SetupGraphics();
	void ComputeVertices();

	Shader* shader;
	unsigned int VAO, VBO, EBO;
	glm::vec3 vertices[4];

	float z;
	glm::vec2 size;
	glm::vec2 origin;
	glm::vec4 color;

	bool compute;
};