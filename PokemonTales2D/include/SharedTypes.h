#pragma once

class Window;
class Game;
class EventManager;

struct SharedContext {
	Game* game;
	Window* window;
	EventManager* eventManager;
};

