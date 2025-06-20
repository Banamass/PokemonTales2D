#pragma once

#include "Pokemon.h"
#include <cmath>
#include <map>

class Box {
public:
	Box(sf::Vector2f l_pos, sf::Vector2i l_boardPos, float l_size, sf::Color l_color = sf::Color::White);
	~Box();

	void Render(Window* win);

	void SetColor(sf::Color l_color);
	void ResetColor();
	sf::Vector2f GetPos();
	sf::Vector2i GetBoardPos();

private:
	sf::Vector2f pos;
	sf::Vector2i boardPos;
	float size;
	sf::RectangleShape sprite;

	sf::Color defaultColor;
};

class Board {
public:
	Board(SharedContext* context);
	virtual ~Board();

	void Update();
	void Render();

	/*Get the coords of the box where the mouse is*/
	sf::Vector2i GetMousePosition();
	sf::Vector2f GetBoxMiddle(sf::Vector2i boxCoord);
	Box* GetBox(sf::Vector2i boxCoord);
	sf::Vector2i GetSize();

	/*Modify pokemonsPos[poke] if the key exist or create a new entry to pokemonsPos and store the position
	The position is in board coords
	If the move is impossible then do nothing and return false, either move and return true*/
	bool SetPokemonPos(Pokemon* poke, sf::Vector2i pos);
	/*Move if the key poke exists in pokemonsPos, either do nothing
	The move is in board coord
	If the move is impossible then do nothing and return false, either move and return true*/
	bool MovePokemon(Pokemon* poke, sf::Vector2i move);
	sf::Vector2i GetPokemonPosition(Pokemon* poke);

	/*Return if the poke poke can be move to position pos*/
	bool CheckMove(Pokemon* poke, sf::Vector2i pos);

	/*Get the pokemon whose area contains pos, if there is no pokemon that match, then return nullptr
	The position is in board coords
	Rq : If there two pokemons that match, then return any of them*/
	Pokemon* GetPokemonFromPos(sf::Vector2i pos);

	void Draw(sf::Shape& shape, sf::Vector2i pos);

private:
	void SetCallbacks();
	void CreateBoxes();
	bool isBoxInBoard(sf::Vector2i boxCoord);

	SharedContext* context;

	float boxSize;
	sf::Vector2i size;
	std::vector<std::vector<Box>> boxes;

	std::map<Pokemon*, sf::Vector2i> pokemonsPos;
};