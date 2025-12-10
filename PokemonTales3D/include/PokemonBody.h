#pragma once

#include "Window.h"
#include "Pokemon.h"
#include "Map.h"
#include "Transitor.h"
#include "Shape.h"

class PokemonBody {
public:
	PokemonBody(SharedContext* l_context, Pokemon* l_pokemon);
	virtual ~PokemonBody();

	void Update(double dt);
	void Render();
	void RenderWithBB(CubeShape* bb);

	Pokemon* GetPokemon();

	//Set the position of the pokemon body movment zone
	void SetZonePosition(glm::vec3 l_pos);
	//Set the movment zone in which the pokemon will be moving, no z-movment
	void SetMovmentZone(glm::vec2 zone);
	//Set the movment zone in which the pokemon will be moving
	void SetMovmentZone(glm::vec3 zone);
	//Set the real position of the pokemon body, without transition
	void SetPosition(glm::vec3 l_pos);

	glm::vec3 GetPosition();
	FloatCube GetFloatCube();

private:
	SharedContext* context;
	Pokemon* pokemon;

	glm::vec3 movmentZone;
	glm::vec3 pos;
	SmoothLinearTransition<glm::vec3> posTransition;
	Timer waitTimer;
};