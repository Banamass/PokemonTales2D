#include "Model.h"

Object::Object() :shader(nullptr) {
	float vertices[] = {
		//positions      //textcoords
		-0.5f,-0.5f,-0.5f, 0.0f, 0.0f,
		0.5f,-0.5f,-0.5f, 1.0f, 0.0f,
		0.5f, 0.5f,-0.5f, 1.0f, 1.0f,
		0.5f, 0.5f,-0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f,-0.5f, 0.0f,1.0f,
		-0.5f,-0.5f,-0.5f, 0.0f,0.0f,
		-0.5f,-0.5f, 0.5f, 0.0f,0.0f,
		0.5f,-0.5f, 0.5f, 1.0f,0.0f,
		0.5f, 0.5f, 0.5f, 1.0f,1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f,1.0f,
		-0.5f,-0.5f, 0.5f, 0.0f,0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f,0.0f,
		-0.5f, 0.5f,-0.5f, 1.0f,1.0f,
		-0.5f,-0.5f,-0.5f, 0.0f,1.0f,
		-0.5f,-0.5f,-0.5f, 0.0f,1.0f,
		-0.5f,-0.5f, 0.5f, 0.0f,0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f,0.0f,
		0.5f, 0.5f, 0.5f, 1.0f,0.0f,
		0.5f, 0.5f,-0.5f, 1.0f,1.0f,
		0.5f,-0.5f,-0.5f, 0.0f,1.0f,
		0.5f,-0.5f,-0.5f, 0.0f,1.0f,
		0.5f,-0.5f, 0.5f, 0.0f,0.0f,
		0.5f, 0.5f, 0.5f, 1.0f,0.0f,
		-0.5f,-0.5f,-0.5f, 0.0f,1.0f,
		0.5f,-0.5f,-0.5f, 1.0f,1.0f,
		0.5f,-0.5f, 0.5f, 1.0f,0.0f,
		0.5f,-0.5f, 0.5f, 1.0f,0.0f,
		-0.5f,-0.5f, 0.5f, 0.0f,0.0f,
		-0.5f,-0.5f,-0.5f, 0.0f,1.0f,
		-0.5f, 0.5f,-0.5f, 0.0f,1.0f,
		0.5f, 0.5f,-0.5f, 1.0f,1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,0.0f,
		0.5f, 0.5f, 0.5f, 1.0f,0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f,0.0f,
		-0.5f, 0.5f,-0.5f, 0.0f,1.0f
	};

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(40.0f), glm::vec3(1.0f, 0.0f, 0.0f));
}
Object::~Object(){
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Object::SetShader(Shader* l_shader) {
	shader = l_shader;
}

void Object::Draw(const glm::mat4& cameraMatrix){
	if (shader == nullptr) {
		std::cout << "Set up a shader for drawing" << std::endl;
		return;
	}
	glm::mat4 transform = cameraMatrix * modelMatrix;
	shader->use();
	shader->SetUniform("transform", glm::value_ptr(transform));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}