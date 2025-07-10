#include "Board.h"
#include "Camera.h"
#include "APlayer.h"

/*------------------Box------------------*/

Box::Box(glm::ivec2 l_pos)
	:pos(l_pos), size(Constants::BOX_SIZE), selectedColor(1.0f, 0.0f, 0.0f){
}

Box::~Box(){}

void Box::Select() {
}

void Box::Unselect() {
}

/*------------------Board------------------*/

Board::Board(glm::vec2 l_size, SharedContext* l_context)
	: context(l_context), size(l_size), boxesDrawable(nullptr)
	, boxDrawable(nullptr), cursorDrawable(nullptr), cursor(glm::vec2(1,1)),
	boxModel("Resources\\Box\\box.obj") {
	instancedModelShader = context->shaderManager->GetShader("InstancedModelShader");
	modelShader = context->shaderManager->GetShader("ModelShader");
	if (instancedModelShader == nullptr) {
		std::cout << "Impossible to create board : instancedModelShader is null" << std::endl;
		return;
	}
	if (modelShader == nullptr) {
		std::cout << "Impossible to create board : modelShader is null" << std::endl;
		return;
	}

	for (int i = 0; i < size.x; i++) {
		boxes.push_back({});
		for (int j = 0; j < size.y; j++) {
			boxes[i].emplace_back(glm::ivec2(i, j));

			float scale = 0.90f;
			Transform* trans = new Transform();
			trans->Scale(glm::vec3(scale, scale, scale));
			trans->Move(glm::vec3(i * (Constants::BOX_SIZE), 0.0f, j * (Constants::BOX_SIZE)));
			boxesTransforms.push_back(trans);
		}
	}

	Drawable::Material mat;
	mat.ambient = 0.3f * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mat.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mat.specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mat.shininess = 32.0f;

	boxDrawable = new Drawable(&boxModel, instancedModelShader);
	boxDrawable->SetMaterial(mat);
	boxesDrawable = new DrawableInstanced(boxDrawable, boxesTransforms);

	cursorDrawable = new Drawable(&boxModel, modelShader);
	mat.ambient = 0.3f * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	mat.diffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	cursorDrawable->SetMaterial(mat);

	cursor.Update(this, glm::ivec2(1, 1));
}
Board::~Board() {
	delete boxesDrawable;
	delete boxDrawable;
	delete cursorDrawable;
	for (Transform* trans : boxesTransforms) {
		delete trans;
	}
}

void Board::Update(double dt) {
	glm::vec3 mouseDir = context->camera->GetMouseDirection();
	glm::vec3 camPos = context->camera->GetPosition();

	if (mouseDir.y < 0) {
		float lambda = camPos.y / mouseDir.y;
		glm::vec3 proj = glm::vec3(camPos.x - lambda * mouseDir.x, 0, camPos.z - lambda * mouseDir.z);
		proj += glm::vec3(Constants::BOX_SIZE / 2.0f, 0, Constants::BOX_SIZE / 2.0f);
		
		glm::ivec2 boardPos((int)(proj.x / Constants::BOX_SIZE), (int)(proj.z / Constants::BOX_SIZE));
		cursor.Update(this, boardPos);
	}
}

void Board::Draw() {
	for (Box* box : cursor.GetBoxes()) {
		glm::ivec2 pos = box->GetPos();
		cursorDrawable->SetPosition(glm::ivec3(
			pos.x * Constants::BOX_SIZE,
			0.0f,
			pos.y * Constants::BOX_SIZE
		));
		context->win->Draw(cursorDrawable);
	}
	context->win->DrawInstanced(boxesDrawable);
}

bool Board::Contain(const glm::ivec2& pos) {
	return pos.x >= 0 && pos.y >= 0 && pos.x < size.x && pos.y < size.y;
}

Box* Board::GetBox(const glm::ivec2& pos) {
	if (!Contain(pos))
		return nullptr;
	return &boxes[pos.x][pos.y];
}