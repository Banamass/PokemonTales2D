#pragma once

#include "Board.h"
#include "EventManager.h"
#include "GUI.h"
#include "Player.h"
#include "GameData.h"

class GameSystem {
public:
	GameSystem(SharedContext* l_context);
	virtual ~GameSystem();

	void Update(double dt);
	void Render();

	void Attack(Pokemon* attacker, Pokemon* attacked);

private:
	void UpdatePlayingPlayer();
	void UpdateHoverPokemon();
	
	void HoverPokemon(Pokemon* poke);
	void UnHoverPokemon();

	SharedContext* context;

	DataManager dataManager;

	std::vector<Pokemon*> playingPokemons;
	Pokemon* hoverPoke;

	Board board;
	Player player;
	Player opponent;
	Player* playingPlayer;
};