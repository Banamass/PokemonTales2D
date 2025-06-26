#pragma once

#include "Window.h"

struct WidgetsAttributs {
	WidgetsAttributs(sf::Vector2f l_size, sf::Color l_color, sf::Vector2f l_pos, float l_frameThickness,
		sf::Color l_frameColor, sf::Color l_frameBackgroundColor) 
	: size(l_size), color(l_color), pos(l_pos), frameThickness(l_frameThickness)
		, frameColor(l_frameColor), frameBackgroundColor(l_frameBackgroundColor){}

	sf::Vector2f size;
	float frameThickness;
	sf::Vector2f pos;
	sf::Color color;
	sf::Color frameColor;
	sf::Color frameBackgroundColor;
};

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

class HealthBar {
public:
	HealthBar(float* l_health, float l_maxHealth, WidgetsAttributs attrs);

	/*Render the health bar at attrs.pos + refPos*/
	void Render(Window* win, sf::Vector2f refPos);

private:
	WidgetsAttributs attrs;

	sf::RectangleShape bar;
	sf::RectangleShape frame;

	float maxHealth;
	float* health;
};