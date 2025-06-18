#pragma once

class Window;
class Game;
class EventManager;
class Board;

struct SharedContext {
	Game* game;
	Window* window;
	EventManager* eventManager;
	Board* board;
};

