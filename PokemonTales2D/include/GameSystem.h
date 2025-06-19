#pragma once

#include "Player.h"

class GameSystem {
public:

	GameSystem(SharedContext* l_context);
	virtual ~GameSystem();

	void Update();
	void Render();

private:
	SharedContext* context;

	Board board;
	Player player;
};