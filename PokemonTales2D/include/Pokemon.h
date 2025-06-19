#pragma once

#include "Window.h"

class Pokemon {
public:
	Pokemon(int size, sf::Color color, sf::Vector2i l_size);
	virtual ~Pokemon();

	void Render(Window* win, sf::Vector2f pos	);

	sf::Vector2i GetSize();
	sf::RectangleShape GetFrame();

private:
	sf::CircleShape sprite;
	sf::RectangleShape frame;

	sf::Vector2i size; //in nb box takes
};