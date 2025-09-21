#include "Pokemon.h"
#include "APlayer.h"

Pokemon::Pokemon(const PokemonData* l_data, APlayer* l_trainer,
	ModelManager* l_modelMgr, ShaderManager* shaderMgr, glm::vec3 l_color)
	: trainer(l_trainer), color(l_color), data(l_data),
	movePool(), modelMgr(l_modelMgr)
{
	if (!modelMgr->RequireResource("Charmander")) {
		std::cout << "Charmander model not found" << std::endl;
	}
	model = modelMgr->GetResource("Charmander");
	sprite = new Drawable(model, shaderMgr->GetShader("ModelShader"));
	OBB = new Drawable(model, shaderMgr->GetShader("ModelShader"));

	health = data->stats.hp;
	for (int i = 0; i < 4; i++) {
		movePool[i] = nullptr;
	}

	Drawable::Material mat;
	mat.ambient = 0.6f * color;
	mat.diffuse = 0.8f * color;
	mat.specular = color;
	mat.shininess = 32.0f;
	
	sprite->SetMaterial(mat);
	sprite->Scale(glm::vec3(0.2f));

	Drawable::Material matOBB;
	matOBB.ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	OBB->SetMaterial(matOBB);
	OBB->Scale(glm::vec3(0.65f));
}
Pokemon::~Pokemon(){
	delete sprite;
	delete OBB;
	for (int i = 0; i < 4; i++) {
		if(movePool[i])
			delete movePool[i];
	}
	modelMgr->ReleaseResource("Charmander");
}

bool Pokemon::TestRayIntersection(
	const glm::vec3& ray_origin,
	const glm::vec3& ray_direction,
	float& interstion_distance) 
{
	return sprite->TestRayIntersection(ray_origin, ray_direction, interstion_distance);
}

void Pokemon::Render(Window* win, glm::ivec2 pos){
	glm::vec3 realPos(pos.x * Constants::BOX_SIZE, 0.4f, pos.y * Constants::BOX_SIZE);
	sprite->SetPosition(realPos);
	OBB->SetPosition(realPos);

	win->Draw(sprite);
}

float Pokemon::GetHealth() { return health; }
float Pokemon::GetMaxHealth() { return data->stats.hp; }
glm::ivec2 Pokemon::GetSize() { return data->size; }
int Pokemon::GetMoveRange() { return data->stats.move; }
APlayer* Pokemon::GetTrainer() { return trainer; }
std::string Pokemon::GetName() { return data->name; }
bool Pokemon::IsKO() { return health <= 0; }
PokemonMove* Pokemon::GetMove(int i) { return movePool[i]; }
std::pair<PokeType, PokeType> Pokemon::GetType() { return data->types; }
void Pokemon::SetMovePool(int i, const MoveData* l_data) { movePool[i] = new PokemonMove(l_data); }

void Pokemon::TakeDamages(float l_damages) { 
	health -= l_damages; 
	health = std::max(health, 0.0f);
	if (IsKO()) {
		trainer->PokemonKO(this);
	}
}