#pragma once

#include <glm/glm.hpp>

//Constants of the game
namespace Constants{
	constexpr float BOX_SIZE = 2.0f;
	constexpr float WIN_WIDTH = 1200.0f;
	constexpr float WIN_HEIGHT = 700.0f;
	constexpr int NB_POKEMON_BY_PLAYER = 4;
}

class Game;
class Window;
class EventManager;
class Camera;
class ShaderManager;
class ModelManager;
class FontManager;
class GameSystem;
class BattleGUI;
class Board;
class DataManager;
class StateManager;

//Structure containing all the main objects of the game, which are shared across the code
struct SharedContext {
	Game* game;
	Window* win;
	EventManager* eventManager;
	Camera* camera;
	ShaderManager* shaderManager;
	ModelManager* modelManager;
	FontManager* fontManager;
	StateManager* stateManager;
	DataManager* gameData;
	GameSystem* gameSystem;
	BattleGUI* gui;
	Board* board;
};

// Enumeration of all supported location
enum Location { Middle, TopLeft, TopRight, BottomRight, BottomLeft,
				MiddleLeft, MiddleRight, MiddleTop, MiddleBottom};

// Enumeration of Direction
enum Direction { Up, Down, Left, Right };

// Enumeration of Orientation
enum Orientation {Hozirontal, Vertical};

//Structure representing a rectangle with int coords
struct IntRect {
	IntRect() : pos(0, 0), size(0, 0) {}
	IntRect(glm::ivec2 l_pos, glm::ivec2 l_size)
		: pos(l_pos), size(l_size) {
	}

	bool Contains(glm::ivec2 v) const;
	bool Intersects(IntRect rect) const;

	glm::ivec2 pos;
	glm::ivec2 size;
};

//Structure representing a rectangle with floating coords
struct FloatRect {
	FloatRect() : pos(0.0f, 0.0f), size(0.0f, 0.0f) {}
	FloatRect(glm::vec2 l_pos, glm::vec2 l_size)
		: pos(l_pos), size(l_size) {
	}

	bool Contains(glm::vec2 v) const;
	bool Intersects(IntRect rect) const;

	glm::vec2 pos;
	glm::vec2 size;
};

//For an object with a size 'size', compute the coords of the point at the location loc
glm::vec2 LocationToPosition(glm::vec2 size, Location loc);

//For an object with a float rect, compute the global coords of the point at the location loc
glm::vec2 LocationToPosition(FloatRect rect, Location loc);