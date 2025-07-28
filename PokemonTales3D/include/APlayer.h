#pragma once

#include "Pokemon.h"
#include <algorithm>
#include <map>
#include <queue>

class Board;
class Box;

class AbstractArea {
public:
	AbstractArea()
		:pos({ -100, -100 }) {
	};
	virtual ~AbstractArea() { Clear(); };

	glm::ivec2 getPosition() { return pos; }
	std::vector<Box*> GetBoxes() { return boxes; }

	virtual bool IsIn(glm::ivec2 pos) = 0;
	virtual void Update(Board* board, glm::ivec2 l_pos) = 0;

protected:
	virtual void Clear();
	void Add(Box* box);

	std::vector<Box*> boxes;
	glm::ivec2 pos;
};

class SquareArea : public AbstractArea {
public:
	SquareArea(glm::ivec2 l_size, Location l_origin = Location::Middle)
		: AbstractArea(), size(l_size), origin(l_origin), originOffset({ 0, 0 })
	{
		compute = true;
		SetRealPosOffset();
		SetIntRect();
	}

	void SetSize(glm::ivec2 size);
	//Get the int rect representing the area
	IntRect GetIntRect();

	bool IsIn(glm::ivec2 pos);
	void Update(Board* board, glm::ivec2 l_pos);

protected:
	void SetIntRect();
	void SetRealPosOffset();

	glm::ivec2 size;
	Location origin;
	glm::ivec2 originOffset;
	IntRect intRect;

	bool compute;
};

class MoveArea : public AbstractArea {
public:
	MoveArea(Pokemon* l_movingPokemon, int nbStep)
		: AbstractArea(), pos(glm::ivec2(-100, -100)), movingPokemon(l_movingPokemon)
	{
		range = nbStep;
	}
	~MoveArea() {}

	// For this class use Distance to have a better complexity
	bool IsIn(glm::ivec2 l_pos);
	void Update(Board* board, glm::ivec2 l_pos);
	// Return the distance of l_pos from origin, -1 if it's unreachable
	int Distance(Board* board, glm::ivec2 l_pos);

protected:
	void Clear();

	int range;
	glm::ivec2 pos;
	std::map<Box*, int> boxDistance; //< 0 corresponds to an unreachable box
	Pokemon* movingPokemon;
};

class APlayer {
public:
	APlayer(SharedContext* l_context);
	virtual ~APlayer();

	void AddPokemon(Pokemon* poke, glm::ivec2 initialPos);

	virtual void Setup() = 0;
	virtual void PlayTurn();

	virtual void Render();

	void PokemonKO(Pokemon* poke);

	bool Playing();
	std::vector<Pokemon*>& GetPokemons();

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
};