#pragma once

class Window;
class EventManager;

struct SharedContext {
	Window* win;
	EventManager* eventManager;
};