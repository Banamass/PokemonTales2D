#pragma once

#include "Map.h"

class World {
public:
	World(SharedContext* l_context);
	~World();

	void Update(double dt);
	void Render();

private:
	SharedContext* context;

	Map map;
};