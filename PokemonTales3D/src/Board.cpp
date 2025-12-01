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
	, boxDrawable(nullptr)
{
	if (!context->modelManager->RequireResource("Box")) {
		std::cout << "Box model not found" << std::endl;
	}
	boxModel = context->modelManager->GetResource("Box");

	context->board = this;

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
	mat.SetLightningColor(glm::vec3(1.0f), 0.3f, 1.0f);
	mat.alpha = 0.4f;

	boxDrawable = new Drawable(boxModel, instancedModelShader);
	boxDrawable->SetMaterial(mat);
	boxesDrawable = new DrawableInstanced(boxDrawable, boxesTransforms);
}
Board::~Board() {
	delete boxesDrawable;
	delete boxDrawable;
	for (Transform* trans : boxesTransforms) {
		delete trans;
	}
	context->modelManager->ReleaseResource("Box");
}

void Board::Update(double dt) {
	
}

void Board::Draw() {
	context->win->DrawInstanced(boxesDrawable);
}

glm::ivec2 Board::GetMousePos() {
	glm::vec3 mouseDir = context->camera->GetMouseDirection();
	glm::vec3 camPos = context->camera->GetPosition();

	if (mouseDir.y < 0) {
		float lambda = camPos.y / mouseDir.y;
		glm::vec3 proj = glm::vec3(camPos.x - lambda * mouseDir.x, 0, camPos.z - lambda * mouseDir.z);
		proj += glm::vec3(Constants::BOX_SIZE / 2.0f, 0, Constants::BOX_SIZE / 2.0f);

		glm::ivec2 boardPos((int)(proj.x / Constants::BOX_SIZE), (int)(proj.z / Constants::BOX_SIZE));
		return boardPos;
	}
	return glm::ivec2(-1.0f, -1.0f);
}

bool Board::Contain(const glm::ivec2& pos) {
	return pos.x >= 0 && pos.y >= 0 && pos.x < size.x && pos.y < size.y;
}

Box* Board::GetBox(const glm::ivec2& pos) {
	if (!Contain(pos))
		return nullptr;
	return &boxes[pos.x][pos.y];
}

glm::ivec2 Board::GetSize() {
	return size;
}

bool Board::SetPokemonPos(Pokemon* poke, glm::ivec2 pos) {
	if (poke == nullptr)
		return false;
	if (!CheckMove(poke, pos))
		return false;
	auto itr = pokemonsPos.find(poke);
	if (itr == pokemonsPos.end()) {
		pokemonsPos.emplace(std::make_pair(poke, pos));
	}
	else {
		pokemonsPos.at(poke) = pos;
	}
	return true;
}
bool Board::MovePokemon(Pokemon* poke, glm::ivec2 move) {
	if (poke == nullptr)
		return false;
	auto itr = pokemonsPos.find(poke);
	if (itr == pokemonsPos.end())
		return false;
	return SetPokemonPos(poke, itr->second + move);
}

bool Board::CheckMove(Pokemon* poke, glm::ivec2 pos) {
	if (poke == nullptr)
		return false;
	IntRect hitbox1(pos, poke->GetSize());
	for (auto& itr : pokemonsPos) {
		if (itr.first == poke)
			continue;
		IntRect hitbox2(itr.second, itr.first->GetSize());
		if (hitbox1.Intersects(hitbox2))
			return false;
	}
	IntRect boardHitBox = IntRect({ 0,0 }, size);
	return boardHitBox.Contains(pos + poke->GetSize() - glm::ivec2({ 1,1 }));
}

glm::ivec2 Board::GetPokemonPosition(Pokemon* poke) {
	return pokemonsPos[poke];
}

IntRect Board::GetPokemonHitbox(Pokemon* poke) {
	auto itr = pokemonsPos.find(poke);
	if (itr == pokemonsPos.end())
		return IntRect();
	glm::ivec2 pokeSize = poke->GetTrainer()->GetPokemonRotateSize(poke);
	return IntRect(itr->second - pokeSize / 2, pokeSize);
}

Pokemon* Board::GetPokemonFromPos(glm::ivec2 pos) {
	for (auto& itr : pokemonsPos) {
		IntRect hitbox = GetPokemonHitbox(itr.first);
		if (hitbox.Contains(pos))
			return itr.first;
	}
	return nullptr;
}
std::vector<Pokemon*> Board::GetPokemonCollision(IntRect hitbox) {
	std::vector<Pokemon*> res;
	for (auto& itr : pokemonsPos) {
		IntRect pokeHitbox = GetPokemonHitbox(itr.first);
		if (pokeHitbox.Intersects(hitbox)) {
			res.push_back(itr.first);
		}
	}
	return res;
}

void Board::RemovePokemon(Pokemon* poke) {
	pokemonsPos.erase(pokemonsPos.find(poke));
}

void Board::ClearAllPokemons() {
	pokemonsPos.clear();
}