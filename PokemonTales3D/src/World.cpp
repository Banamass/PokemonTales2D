#include "World.h"

World::World(SharedContext* l_context)
	: context(l_context), map(l_context, "Resources\\GameData\\Map\\map.json"), playerBody(l_context),
	gravity(23.0f), cameraRotation(-1,-1)
{
	
}
World::~World(){
}

void World::Update(double dt){
	playerBody.Accelerate(glm::vec3(0.0f, -gravity, 0.0f));

	map.Update(dt);
	playerBody.Update(dt);
	UpdateCamera();
}
void World::Render(){
	map.Render();
	playerBody.Render();
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
}

void World::UpdateCamera() {
	glm::vec3 playerPos = playerBody.GetPosition();
	glm::vec3 cameraPos = playerPos + glm::vec3(cameraRotation.x * 8.0f, 4.0f, cameraRotation.y * 8.0f);
	context->camera->SetPosition(cameraPos);
	context->camera->SetFront(playerPos - cameraPos);
}
