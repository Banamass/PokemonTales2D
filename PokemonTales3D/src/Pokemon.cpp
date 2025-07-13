#include "Pokemon.h"

Pokemon::Pokemon(const std::string& l_name, glm::ivec2 l_size, APlayer* l_trainer)
	: name(l_name), size(l_size), trainer(l_trainer)
{
	health = 100.0f;
	moveRange = 5;
}
Pokemon::~Pokemon(){}

void Pokemon::Render(Window* win, glm::ivec2 pos){}

glm::ivec2 Pokemon::GetSize() { return size; }
int Pokemon::GetMoveRange() { return moveRange; }
void Pokemon::SetMoveRange(int l_range) { moveRange = l_range; }
APlayer* Pokemon::GetTrainer() { return trainer; }
std::string Pokemon::GetName() { return name; }
bool Pokemon::IsKO() { return health <= 0; }

void Pokemon::TakeDamages(float l_damages) { health -= l_damages; }