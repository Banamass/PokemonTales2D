#include "World.h"

World::World(SharedContext* l_context)
	: context(l_context), map(l_context, "Resources\\GameData\\Map\\map.json"), playerBody(l_context),
	gravity(23.0f), cameraTransition(glm::vec3(0.0f), glm::vec3(0.0f), 5.0f, 6.0f)
{
	SetCameraRotation(glm::vec2(-1, -1));

	Pokemon* p = new Pokemon(context->gameData->GetPokemonData("Charmander"),
		nullptr, context->modelManager, context->shaderManager, glm::vec3(1.0f));
	auto newP = wildPokemons.emplace(std::piecewise_construct
		, std::forward_as_tuple(p), std::forward_as_tuple(context, p));
	newP.first->second.SetPosition(glm::vec3(17.5f, 0.0f, 9.5f));
	newP.first->second.SetMovmentZone(glm::vec2(7.0f));

	p = new Pokemon(context->gameData->GetPokemonData("Bulbasaur"),
		nullptr, context->modelManager, context->shaderManager, glm::vec3(1.0f));
	newP = wildPokemons.emplace(std::piecewise_construct
		, std::forward_as_tuple(p), std::forward_as_tuple(context, p));
	newP.first->second.SetPosition(glm::vec3(5.0f, 0.0f, 5.0f));
	newP.first->second.SetMovmentZone(glm::vec2(3.0f));

	p = new Pokemon(context->gameData->GetPokemonData("Squirtle"),
		nullptr, context->modelManager, context->shaderManager, glm::vec3(1.0f));
	newP = wildPokemons.emplace(std::piecewise_construct
		, std::forward_as_tuple(p), std::forward_as_tuple(context, p));
	newP.first->second.SetPosition(glm::vec3(0.5f, 0.0f, 0.5f));
	newP.first->second.SetMovmentZone(glm::vec2(3.0f));
}
World::~World(){
	for (auto& e : wildPokemons) {
		delete e.first;
	}
}

void World::Update(double dt){
	playerBody.Accelerate(glm::vec3(0.0f, -gravity, 0.0f));

	map.Update(dt);
	playerBody.Update(dt);
	for (auto& e : wildPokemons) {
		e.second.Update(dt);
	}
	UpdateCamera(dt);
}
void World::Render(){
	map.Render();
	playerBody.Render();
	for (auto& e : wildPokemons) {
		e.second.Render();
	}
}

void World::SetCameraRotation(glm::vec2 rot) {
	if (rot.x >= 0)
		cameraRotation.x = 1;
	else
		cameraRotation.x = -1;
	if (rot.y >= 0)
		cameraRotation.y = 1;
	else
		cameraRotation.y = -1;

	playerBody.SetFrontMove(-glm::vec3(cameraRotation.x, 0.0f, cameraRotation.y));
}

void World::RotateCamera(bool r) {
	if (cameraRotation == glm::ivec2(-1, -1))
		cameraRotation = r ? glm::ivec2(1, -1) : glm::ivec2(-1, 1);
	else if (cameraRotation == glm::ivec2(1, 1))
		cameraRotation = r ? glm::ivec2(-1, 1) : glm::ivec2(1, -1);
	else if (cameraRotation == glm::ivec2(-1, 1))
		cameraRotation = r ? glm::ivec2(-1, -1) : glm::ivec2(1, 1);
	else if (cameraRotation == glm::ivec2(1, -1))
		cameraRotation = r ? glm::ivec2(1, 1) : glm::ivec2(-1, -1);

	playerBody.SetFrontMove(-glm::vec3(cameraRotation.x, 0.0f, cameraRotation.y));
}

void World::UpdateCamera(double dt) {
	cameraTransition.Update(dt);
	glm::vec3 playerPos = playerBody.GetPosition();
	cameraTransition.target = playerPos + glm::vec3(cameraRotation.x * 8.0f, 6.0f, cameraRotation.y * 8.0f);
	context->camera->SetPosition(cameraTransition.current);
	context->camera->SetFront(playerPos - cameraTransition.target);
}
