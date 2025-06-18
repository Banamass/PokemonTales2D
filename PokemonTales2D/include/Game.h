#pragma once

#include "GameSystem.h"

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
	GameSystem gameSystem;

	SharedContext context;
};

