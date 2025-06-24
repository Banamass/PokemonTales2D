#pragma once

#include "Widgets.h"

class APlayer;

class Pokemon {
public:
	Pokemon(const std::string& l_name,int size, sf::Color color, sf::Vector2i l_size, APlayer* l_trainer);
	virtual ~Pokemon();

	void Render(Window* win, sf::Vector2f pos	);

	sf::Vector2i GetSize();
	sf::RectangleShape GetFrame();
	int GetMoveRange();
	void SetMoveRange(int l_range);
	APlayer* GetTrainer();
	std::string GetName();
	
	void TakeDamages(float l_damages);

private:
	sf::CircleShape sprite;
	sf::RectangleShape frame;

	sf::Vector2i size; //in nb box takes
	int moveRange;
	float health;
	std::string name;

	APlayer* trainer;
};