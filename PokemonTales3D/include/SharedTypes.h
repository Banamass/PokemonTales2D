#pragma once

namespace Constants{
	constexpr float BOX_SIZE = 2.0f;
}

class Window;
class EventManager;
class Camera;
class ShaderManager;

struct SharedContext {
	Window* win;
	EventManager* eventManager;
	Camera* camera;
	ShaderManager* shaderManager;
};