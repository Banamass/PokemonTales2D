#pragma once

#include "Board.h"
#include "EventManager.h"
#include "GameGUI.h"
#include "Player.h"
#include "GameData.h"
#include "OptionsState.h"

#include <cstdlib>
#include "time.h"

//This class represent a given battle, and handles all the components of that battle
class GameSystem {
public:
	//Set up for starting the battle
	GameSystem(SharedContext* l_context);
	virtual ~GameSystem();

	//Start the battle
	void StartBattle();

	//Update method
	void Update(double dt);
	//Render method
	void Render();

	//Perform the attack between an attacker pokemon and a attacked pokemon, with the move move 
	void Attack(Pokemon* attacker, Pokemon* attacked, PokemonMove* move);
	//Compute the damages of the attack between an attacker pokemon and a attacked pokemon, with the move move 
	float ComputeDamages(Pokemon* attacker, Pokemon* attacked, PokemonMove* move);

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