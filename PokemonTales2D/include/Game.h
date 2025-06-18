#pragma once

#include "Board.h"

class Game
{
public:
	Game();
	virtual ~Game();

	void Update();
	void LateUpdate();
	void Render();

	Window* GetWindow();
	sf::Time GetElapsed();
	void RestartClock();

protected:
	sf::Clock clock;
	sf::Time elapsed;
	float frametime;

	Window window;
	Board board;

	SharedContext context;
};

