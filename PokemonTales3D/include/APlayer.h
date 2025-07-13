#pragma once

#include "Pokemon.h"
#include <algorithm>

class Board;
class Box;

class AbstractArea {
public:
	AbstractArea(glm::vec4 l_selectedColor)
		:selectedColor(l_selectedColor), pos({ -100, -100 }) {
	};
	virtual ~AbstractArea() { Clear(); };

	glm::ivec2 getPosition() { return pos; }
	std::vector<Box*> GetBoxes() { return boxes; }

	virtual bool IsIn(glm::ivec2 pos) = 0;
	virtual void Update(Board* board, glm::ivec2 l_pos) = 0;
	virtual void Render(Window* win) {};

protected:
	virtual void Clear();
	void Add(Box* box);

	std::vector<Box*> boxes;
	glm::ivec2 pos;

	glm::vec4 selectedColor;
};

class SquareArea : public AbstractArea {
public:
	SquareArea(glm::ivec2 l_size, glm::vec4 l_selectedColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Location l_origin = Location::Middle)
		: AbstractArea(l_selectedColor), size(l_size), origin(l_origin), originOffset({ 0, 0 })
	{
		SetRealPosOffset();
		SetIntRect();
	}

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
};

class APlayer {
public:
	APlayer(SharedContext* l_context);
	virtual ~APlayer();

	virtual void Setup() = 0;
	virtual void PlayTurn();

	void PokemonKO(Pokemon* poke);

	bool Playing();
	std::vector<Pokemon*>& GetPokemons();

protected:
	SharedContext* context;

	bool isPlaying;
	std::vector<Pokemon*> pokemons;
};