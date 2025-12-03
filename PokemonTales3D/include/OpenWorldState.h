#pragma once

#include "StateManager.h"
#include "EventManager.h"

class OpenWorldState : public State {
public:
	OpenWorldState(SharedContext* l_context);
	~OpenWorldState();

	void Update(double dt);
	void Render();

private:
	void KeyCallback(CallbackData data);

};