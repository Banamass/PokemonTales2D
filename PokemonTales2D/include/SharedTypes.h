#pragma once

class Constants {
public:
	static const int BOX_SIZE = 50;
	static const int GAP_SIZE = 2.0;
};

enum Location { Middle, TopLeft, TopRight, BottomRight, BottomLeft };

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

