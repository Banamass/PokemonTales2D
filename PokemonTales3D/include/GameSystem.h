#pragma once

#include "Board.h"

class GameSystem {
public:
	GameSystem(SharedContext* l_context);
	virtual ~GameSystem();

	void Update(double dt);
	void Render();

private:
	SharedContext* context;

	Board board;
};