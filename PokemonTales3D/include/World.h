#pragma once

#include "Map.h"
#include "PlayerBody.h"

class World {
public:
	World(SharedContext* l_context);
	~World();

	void Update(double dt);
	void Render();

	//Set the camera Rotation
	void SetCameraRotation(glm::vec2 rot);
	//Rotate the camera (true -> left, fasle -> right)
	void RotateCamera(bool r);

private:
	void UpdateCamera();
	glm::ivec2 cameraRotation;

	SharedContext* context;

	Map map;
	PlayerBody playerBody;

	float gravity;
};