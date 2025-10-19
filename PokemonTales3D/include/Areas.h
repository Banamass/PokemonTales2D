#pragma once

#include "Board.h"
#include <algorithm>
#include <map>
#include <queue>

//This abstract class represents an area of the board
class AbstractArea {
public:
	AbstractArea()
		:pos({ -100, -100 }) {
	};
	virtual ~AbstractArea() { Clear(); };

	//Get the position of the area
	glm::ivec2 getPosition() { return pos; }
	//Get boxes that make up the area
	std::vector<Box*> GetBoxes() { return boxes; }

	//Return if a position is in the area
	virtual bool IsIn(glm::ivec2 pos) = 0;
	//Update the area representation on the board, with a new pos
	virtual void Update(Board* board, glm::ivec2 l_pos) = 0;

protected:
	virtual void Clear();
	void Add(Box* box);

	std::vector<Box*> boxes;
	glm::ivec2 pos;
};

//This class implements Abstract area for a square area
class SquareArea : public AbstractArea {
public:
	SquareArea(glm::ivec2 l_size, Location l_origin = Location::Middle)
		: AbstractArea(), size(l_size), origin(l_origin), originOffset({ 0, 0 })
	{
		compute = true;
		SetRealPosOffset();
		SetIntRect();
	}

	//Set the size of the area
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

//This class implements Abstract area for an area representing the moving zone of an entity on the board
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
	// Return the distance of l_pos from the area origin, -1 if it's unreachable
	int Distance(Board* board, glm::ivec2 l_pos);

protected:
	void Clear();

	int range;
	glm::ivec2 pos;
	std::map<Box*, int> boxDistance; //< 0 corresponds to an unreachable box
	Pokemon* movingPokemon;
};
