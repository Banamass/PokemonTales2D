#pragma once

#include "Window.h"

class TextField {
public:
	TextField(sf::Vector2f l_backSize, unsigned int l_fontSize, sf::Vector2f l_pos);
	virtual ~TextField();

	void AddMessage(const std::string& mess);

	void Render(Window* win);

private:
	void PushMessage(sf::Text& mess);

	unsigned int fontSize;
	sf::Font font;
	sf::Vector2f backSize;
	sf::Vector2f padding;

	sf::RectangleShape background;
	sf::Vector2f pos;

	sf::FloatRect textBox;
	std::vector<sf::Text> messages;
};