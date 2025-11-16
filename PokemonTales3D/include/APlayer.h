#pragma once

#include "Pokemon.h"
#include "Areas.h"

//This class represents an abstract player, with all the methods that a player has to perform
class APlayer {
public:
	APlayer(SharedContext* l_context);
	virtual ~APlayer();

	//Add a new pokemon and return if the adding is performed
	bool AddPokemon(Pokemon* poke, glm::ivec2 initialPos);

	//Setting up the player
	virtual void Setup() = 0;
	//Play a turn
	virtual void PlayTurn();
	//End turn
	virtual void EndTurn();
	//Update the state of the player
	virtual void Update(double dt) {}

	//Render method for the player
	virtual void Render();

	//Notify that poke is KO
	void PokemonKO(Pokemon* poke);

	//Getters
	
	bool Playing();
	std::vector<Pokemon*>& GetPokemons();
	void SetPlayerColor(glm::vec3 color);

protected:
	SharedContext* context;

	bool isPlaying;
	std::vector<Pokemon*> pokemons;

	struct PokemonState {
		int nbStepLeft;
		int nbMove;
		bool lock;
	};
	std::unordered_map<Pokemon*, PokemonState> pokemonState;

	glm::vec3 playerColor;
	Drawable pokemonsMark;
};