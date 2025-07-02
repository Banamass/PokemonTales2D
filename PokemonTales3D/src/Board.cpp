#include "Board.h"
#include "Camera.h"

/*------------------Box------------------*/

Box::Box(glm::vec2 l_pos, Model* l_model, Shader* l_shader)
	:pos(l_pos), size(Constants::BOX_SIZE), sprite(l_model, l_shader),
	unselectedColor(1.0f, 1.0f, 1.0f), selectedColor(1.0f, 0.0f, 0.0f){

	float scale = 0.60f;
	sprite.Scale(glm::vec3(scale, scale, scale));
	sprite.Move(glm::vec3(pos.x, 0.0f, pos.y));
	SetColor(unselectedColor);
}

Box::~Box(){}

void Box::Draw(Window* win) {
	win->Draw(sprite);
}

void Box::Select() {
	SetColor(selectedColor);
}

void Box::Unselect() {
	SetColor(unselectedColor);
}

void Box::SetColor(glm::vec3 color) {
	Drawable::Material mat;
	mat.ambient = 0.3f * color;
	mat.diffuse = color;
	mat.specular = color;
	mat.shininess = 32.0f;
	sprite.SetMaterial(mat);
}

/*------------------Board------------------*/

Board::Board(glm::vec2 l_size, SharedContext* l_context)
	: context(l_context), size(l_size), selectedBox(nullptr),
	boxModel("Resources\\Box\\box.obj"){
	boxShader = context->shaderManager->GetShader("ModelShader");
	if (boxShader == nullptr) {
		std::cout << "Impossible to create board : boxShader is null" << std::endl;
		return;
	}
	for (int i = 0; i < size.x; i++) {
		boxes.push_back({});
		for (int j = 0; j < size.y; j++) {
			boxes[i].emplace_back(glm::vec2(i * (Constants::BOX_SIZE), j * (Constants::BOX_SIZE)), &boxModel, boxShader);
		}
	}
}
Board::~Board() {}

void Board::Update(double dt) {
	glm::vec3 mouseDir = context->camera->GetMouseDirection();
	glm::vec3 camPos = context->camera->GetPosition();

	if (mouseDir.y < 0) {
		float lambda = camPos.y / mouseDir.y;
		glm::vec3 proj = glm::vec3(camPos.x - lambda * mouseDir.x, 0, camPos.z - lambda * mouseDir.z);

		glm::ivec2 boardPos((int)(proj.x / Constants::BOX_SIZE), (int)(proj.z / Constants::BOX_SIZE));
		Box* temp = selectedBox;
		selectedBox = GetBox(boardPos);
		if (selectedBox != temp) {
			if(selectedBox != nullptr)
				selectedBox->Select();
			if (temp != nullptr)
				temp->Unselect();
		}
	}
}

void Board::Draw() {
	for (int i = 0; i < size.x; i++) {
		boxes.push_back({});
		for (int j = 0; j < size.y; j++) {
			boxes[i][j].Draw(context->win);
		}
	}
}

bool Board::Contain(const glm::ivec2& pos) {
	return pos.x >= 0 && pos.y >= 0 && pos.x < size.x && pos.y < size.y;
}

Box* Board::GetBox(const glm::ivec2& pos) {
	if (!Contain(pos))
		return nullptr;
	return &boxes[pos.x][pos.y];
}