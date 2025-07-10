#pragma once

#include <glm/glm.hpp>

namespace Constants{
	constexpr float BOX_SIZE = 2.0f;
	constexpr float WIN_WIDTH = 800.0f;
	constexpr float WIN_HEIGHT = 600.0f;
}

class Window;
class EventManager;
class Camera;
class ShaderManager;
class GameSystem;

struct SharedContext {
	Window* win;
	EventManager* eventManager;
	Camera* camera;
	ShaderManager* shaderManager;
	GameSystem* gameSystem;
};

enum Location { Middle, TopLeft, TopRight, BottomRight, BottomLeft };

struct IntRect {
	IntRect() : pos(0,0), size(0,0){}
	IntRect(glm::ivec2 l_pos, glm::ivec2 l_size)
		: pos(l_pos), size(l_size){}

	bool Contains(glm::ivec2 v) const {
		return (v.x >= pos.x && v.x <= pos.x + size.x
			&& v.y >= pos.y && v.y <= pos.y + size.y);
	}
	
	bool Intersects(IntRect rect) const {
		return !(pos.x > rect.pos.x + rect.size.x
			|| rect.pos.x > pos.x + size.x
			|| pos.y > rect.pos.y + rect.size.y
			|| rect.pos.y > pos.y + size.y);
	}

	glm::ivec2 pos;
	glm::ivec2 size;
};