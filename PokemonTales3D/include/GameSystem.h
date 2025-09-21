#pragma once

#include "Board.h"
#include "EventManager.h"
#include "GUI.h"
#include "Player.h"
#include "GameData.h"
#include "OptionsState.h"

#include <cstdlib>
#include "time.h"

class GameSystem {
public:
	GameSystem(SharedContext* l_context);
	virtual ~GameSystem();

	void Update(double dt);
	void Render();

	void Attack(Pokemon* attacker, Pokemon* attacked, PokemonMove* move);
	float ComputeDamages(Pokemon* attacker, Pokemon* attacked, PokemonMove* move);

	void StartBattle();

private:
	void UpdatePlayingPlayer();
	void UpdateHoverPokemon();
	
	void HoverPokemon(Pokemon* poke);
	void UnHoverPokemon();

	SharedContext* context;

	Pokemon* hoverPoke;

	glm::ivec2 boardSize;
	Board board;
	Player player;
	Player opponent;
	Player* playingPlayer;
};