#pragma once

#include "Map.h"
#include "PlayerBody.h"
#include "PokemonBody.h"
#include "Transitor.h"

class World {
public:
	World(SharedContext* l_context);
	~World();

	void Update(double dt);
	void Render();

	//Set the camera Rotation
	void SetCameraRotation(glm::vec2 rot);
	//Rotate the camera (true -> left, falsee -> right)
	void RotateCamera(bool r);

private:
	void UpdateCamera(double dt);
	glm::ivec2 cameraRotation;
	SmoothLinearTransition<glm::vec3> cameraTransition;

	SharedContext* context;

	Map map;
	PlayerBody playerBody;
	std::unordered_map<Pokemon*, PokemonBody> wildPokemons;

	float gravity;
};