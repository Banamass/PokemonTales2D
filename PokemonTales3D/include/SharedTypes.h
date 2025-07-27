#pragma once

#include <glm/glm.hpp>

namespace Constants{
	constexpr float BOX_SIZE = 2.0f;
	constexpr float WIN_WIDTH = 1200.0f;
	constexpr float WIN_HEIGHT = 900.0f;
}

class Window;
class EventManager;
class Camera;
class ShaderManager;
class GameSystem;
class GUI;
class Board;
class DataManager;

struct SharedContext {
	Window* win;
	EventManager* eventManager;
	Camera* camera;
	ShaderManager* shaderManager;
	GameSystem* gameSystem;
	GUI* gui;
	Board* board;
	DataManager* gameData;
};

enum Location { Middle, TopLeft, TopRight, BottomRight, BottomLeft };

glm::vec2 LocationToPosition(glm::vec2 size, Location loc);

struct IntRect {
	IntRect() : pos(0,0), size(0,0){}
	IntRect(glm::ivec2 l_pos, glm::ivec2 l_size)
		: pos(l_pos), size(l_size){}

	bool Contains(glm::ivec2 v) const;
	bool Intersects(IntRect rect) const;

	glm::ivec2 pos;
	glm::ivec2 size;
};

struct FloatRect {
	FloatRect() : pos(0, 0), size(0, 0) {}
	FloatRect(glm::ivec2 l_pos, glm::ivec2 l_size)
		: pos(l_pos), size(l_size) {
	}

	bool Contains(glm::ivec2 v) const;
	bool Intersects(IntRect rect) const;

	glm::vec2 pos;
	glm::vec2 size;
};