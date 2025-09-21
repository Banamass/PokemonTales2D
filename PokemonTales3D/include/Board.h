#pragma once

#include <math.h>
#include <map>

#include "SharedTypes.h"
#include "Window.h"
#include "Model.h"
#include "APlayer.h"

class Box {
public:
	Box(glm::ivec2 l_pos);
	~Box();

	void Select();
	void Unselect();

	glm::ivec2 GetPos() { return pos; }

private:
	glm::vec3 selectedColor;

	glm::ivec2 pos;
	float size;
};

class Board {
public:
	Board(glm::vec2 l_size, SharedContext* context);
	~Board();

	void Update(double dt);
	void Draw();

	glm::ivec2 GetMousePos();
	bool Contain(const glm::ivec2& pos);
	Box* GetBox(const glm::ivec2& pos);
	glm::ivec2 GetSize();

	/*Modify pokemonsPos[poke] if the key exist or create a new entry to pokemonsPos and store the position
	The position is in board coords
	If the move is impossible then do nothing and return false, either move and return true*/
	bool SetPokemonPos(Pokemon* poke, glm::ivec2 pos);
	/*Move if the key poke exists in pokemonsPos, either do nothing
	The move is in board coord
	If the move is impossible then do nothing and return false, either move and return true*/
	bool MovePokemon(Pokemon* poke, glm::ivec2 move);
	glm::ivec2 GetPokemonPosition(Pokemon* poke);
	/*Return the hit box of a pokemon
	If the poke isn't on the board, return an empty intrect*/
	IntRect GetPokemonHitbox(Pokemon* poke);

	/*Return if the poke poke can be move to position pos*/
	bool CheckMove(Pokemon* poke, glm::ivec2 pos);
	/*Return the list of pokemons who are touched by the hitbox*/
	std::vector<Pokemon*> GetPokemonCollision(IntRect hitbox);

	/*Get the pokemon whose area contains pos, if there is no pokemon that match, then return nullptr
	The position is in board coords
	Rq : If there two pokemons that match, then return any of them*/
	Pokemon* GetPokemonFromPos(glm::ivec2 pos);

	/*Remove pokemon from the board*/
	void RemovePokemon(Pokemon* poke);
	void ClearAllPokemons();
private:
	SharedContext* context;

	Model* boxModel;
	Shader* instancedModelShader;
	Shader* modelShader;

	glm::ivec2 size;

	std::vector<std::vector<Box>> boxes;
	std::vector<Transform*> boxesTransforms;
	Drawable* boxDrawable;
	DrawableInstanced* boxesDrawable;

	std::map<Pokemon*, glm::ivec2> pokemonsPos;
};