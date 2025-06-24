#pragma once

#include "Player.h"
#include "PlayerAI.h"
#include "Widgets.h"

class GameSystem {
public:

	GameSystem(SharedContext* l_context);
	virtual ~GameSystem();

	void Update();
	void Render();

	void Attack(Pokemon* attacker, Pokemon* attacked);
	
private:
	SharedContext* context;

	Board board;
	Player player;
	PlayerAI opponent;
	APlayer* playingPlayer;

	TextField gameInfoField;
};