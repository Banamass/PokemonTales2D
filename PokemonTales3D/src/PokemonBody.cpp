#include "PokemonBody.h"

PokemonBody::PokemonBody(SharedContext* l_context, Pokemon* l_pokemon)
	: context(l_context), pokemon(l_pokemon), pos(0.0f), movmentZone(0.0f),
	posTransition(glm::vec3(0.0f), glm::vec3(0.0f), 1.6f, 0.0f), waitTimer(1.0)
{}
PokemonBody::~PokemonBody(){}

void PokemonBody::Update(double dt){
	if (waitTimer.Update(dt)) {
		if (posTransition.Update(dt) || context->map->GetCollision(posTransition.current)
			|| !context->map->GetIsOnCube(posTransition.current)) {
			glm::vec3 sign = glm::vec3(rand() % 2 == 0 ? 1 : -1, 
				rand() % 2 == 0 ? 1 : -1, 
				rand() % 2 == 0 ? 1 : -1);
			glm::vec3 delta(sign.x * movmentZone.x / 2.0f * rand() / RAND_MAX, 
				sign.y * movmentZone.y / 2.0f * rand() / RAND_MAX,
				sign.z * movmentZone.z / 2.0f * rand() / RAND_MAX);
			glm::vec3 newPos = pos + delta;
			if(!context->map->GetCollision(newPos) || !context->map->GetIsOnCube(newPos)){
				posTransition.target = newPos;
				waitTimer.Reset();
			}
		}
	}
}
void PokemonBody::Render(){
	pokemon->Render(context->win, posTransition.current);
}
void PokemonBody::RenderWithBB(CubeShape* bb) {
	pokemon->RenderWithBB(context->win, posTransition.current, bb);
}

void PokemonBody::SetZonePosition(glm::vec3 l_pos) { pos = l_pos; }
void PokemonBody::SetMovmentZone(glm::vec2 zone) { movmentZone = glm::vec3(zone.x, 0.0f, zone.y); }
void PokemonBody::SetMovmentZone(glm::vec3 zone) { movmentZone = zone; }
void PokemonBody::SetPosition(glm::vec3 l_pos) {
	posTransition.current = l_pos;
	posTransition.target = l_pos;
	pos = l_pos;
}
glm::vec3 PokemonBody::GetPosition() { return posTransition.current; }

Pokemon* PokemonBody::GetPokemon() { return pokemon; }
FloatCube PokemonBody::GetFloatCube() {
	return pokemon->GetFloatCube();
}