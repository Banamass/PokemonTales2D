#include "Pokemon.h"
#include "APlayer.h"

/*------------------PokemonStatus------------------*/

PokemonStatus::PokemonStatus(const PokemonData* l_data) : data(l_data) {
	damages = 0;
	level = 10;
	for (int i = 0; i < 4; i++) {
		movePool[i] = nullptr;
	}
	for (int i = 0; i < Constants::NB_STATS; i++) {
		IV[i] = 0;
		EV[i] = 0;
	}
}

int PokemonStatus::GetStat(Stat stat) {
	if (stat == Stat::Hp)
		return ((2 * data->stats[stat] + IV[stat] + EV[stat] / 4) * level
			/ 100) + level + 10;
	else
		return (((2 * data->stats[stat] + IV[stat] + EV[stat] / 4) * level
			/ 100) + 5) * 1; //Nature
}

int PokemonStatus::GetHealth() {
	return GetStat(Stat::Hp) - damages;
}

void PokemonStatus::TakeDamages(int l_damages) {
	damages = std::min(GetStat(Stat::Hp), damages + l_damages);
}

PokemonStatus::~PokemonStatus() {
	for (int i = 0; i < 4; i++) {
		if (movePool[i])
			delete movePool[i];
	}
}



/*------------------Pokemon------------------*/

Pokemon::Pokemon(const PokemonData* l_data, APlayer* l_trainer,
	ModelManager* l_modelMgr, ShaderManager* shaderMgr, glm::vec3 l_color)
	: trainer(l_trainer), color(l_color), status(l_data), modelMgr(l_modelMgr)
{
	if (!modelMgr->RequireResource("Charmander")) {
		std::cout << "Charmander model not found" << std::endl;
	}
	model = modelMgr->GetResource("Charmander");
	sprite = new Drawable(model, shaderMgr->GetShader("ModelShader"));
	OBB = new Drawable(model, shaderMgr->GetShader("ModelShader"));

	if (l_data->name == "Bulbasaur")
		color = glm::vec3(0.0f, 1.0f, 0.0f);
	if (l_data->name == "Charmander")
		color = glm::vec3(1.0f, 0.0f, 0.0f);
	if (l_data->name == "Squirtle")
		color = glm::vec3(0.0f, 0.0f, 1.0f);

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

int Pokemon::GetHealth() { return status.GetHealth(); }
int Pokemon::GetMaxHealth() { return status.GetStat(Stat::Hp); }
glm::ivec2 Pokemon::GetSize() { return status.data->size; }
int Pokemon::GetMoveRange() { return status.GetStat(Stat::Move); }
APlayer* Pokemon::GetTrainer() { return trainer; }
std::string Pokemon::GetName() { return status.data->name; }
bool Pokemon::IsKO() { return GetHealth() <= 0; }
PokemonMove* Pokemon::GetMove(int i) { return status.movePool[i]; }
std::pair<PokeType, PokeType> Pokemon::GetType() { return status.data->types; }
void Pokemon::SetMovePool(int i, const MoveData* l_data) { 
	if (status.movePool[i])
		delete status.movePool[i];
	status.movePool[i] = new PokemonMove(l_data); 
}
PokemonStatus& Pokemon::GetStatus() { return status; }

void Pokemon::TakeDamages(int l_damages) { 
	status.TakeDamages(l_damages);
	if (IsKO()) {
		trainer->PokemonKO(this);
	}
}