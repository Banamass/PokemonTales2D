#pragma once

#include "Board.h"

class GameSystem {
public:

	GameSystem(SharedContext* l_context);
	~GameSystem();

	void Update();
	void Render();

private:
	SharedContext* context;

	Board board;
};