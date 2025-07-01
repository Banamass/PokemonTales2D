#include "Board.h"

/*------------------Box------------------*/

Box::Box(glm::vec2 l_pos, Model* l_model, Shader* l_shader)
	:pos(l_pos), size(Constants::BOX_SIZE), sprite(l_model, l_shader) {
	float scale = 0.60f;
	sprite.Scale(glm::vec3(scale, scale, scale));
	sprite.Move(glm::vec3(pos.x, 0.0f, pos.y));
	Drawable::Material mat;
	mat.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	mat.diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	mat.specular = glm::vec3(0.5f, 0.5f, 0.5f);
	mat.shininess = 32.0f;
	sprite.SetMaterial(mat);
}

Box::~Box(){}

void Box::Draw(Window* win) {
	win->Draw(sprite);
}

/*------------------Board------------------*/

Board::Board(glm::vec2 l_size, SharedContext* l_context)
	: context(l_context), size(l_size),
	boxModel("Resources\\Box\\box.obj"){
	boxShader = context->shaderManager->GetShader("ModelShader");
	if (boxShader == nullptr) {
		std::cout << "Impossible to create board : boxShader is null" << std::endl;
		return;
	}
	for (int i = 0; i < size.x; i++) {
		for (int j = 0; j < size.y; j++) {
			boxes.emplace_back(glm::vec2(i * (Constants::BOX_SIZE), j * (Constants::BOX_SIZE)), &boxModel, boxShader);
		}
	}
}
Board::~Board() {}

void Board::Update(double dt) {

}

void Board::Draw() {
	for (Box& box : boxes) {
		box.Draw(context->win);
	}
}