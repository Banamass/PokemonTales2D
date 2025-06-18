#pragma once

#include "Window.h"
#include <cmath>

class Box {
public:
	Box(sf::Vector2f l_pos, float l_size, sf::Color l_color = sf::Color::White);
	~Box();

	void Render(Window* win);

	void SetColor(sf::Color l_color);
	void ResetColor();

private:
	sf::Vector2f pos;
	float size;
	sf::RectangleShape sprite;

	sf::Color defaultColor;
};

class SelectedBoxArea {
public:
	enum Shape {Square, Cross, Diamant};

	SelectedBoxArea() : center(-100,-100), shape(Square), size(1){}
	SelectedBoxArea(sf::Vector2i l_center, Shape l_shape, int l_size)
		: center(l_center), shape(l_shape), size(l_size){}

	void Update(Board* board, sf::Vector2i centerCoord);
	void Clear();

	void SetSize(int l_size);
	void SetShape(Shape l_shape);
	void SetSelectedColor(sf::Color l_color);

private:
	void UpdateSquare(Board* board, sf::Vector2i centerCoord);
	void UpdateCross(Board* board, sf::Vector2i centerCoord);
	void UpdateDiamant(Board* board, sf::Vector2i centerCoord);
	void Add(Box* box);

	std::vector<Box*> boxes;
	sf::Vector2i center;
	Shape shape;
	int size;
	sf::Color selectedColor;
};

class Board {
public:
	Board(SharedContext* context);
	~Board();

	void Update();
	void Render();

	Box* GetBox(sf::Vector2i);

private:
	void SetCallbacks();
	void CreateBoxes();
	bool isBoxInBoard(sf::Vector2i boxCoord);

	void UpdateCursor();

	SharedContext* context;

	float boxSize;
	sf::Vector2i size;
	std::vector<std::vector<Box>> boxes;

	SelectedBoxArea cursor;
};