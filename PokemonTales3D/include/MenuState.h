#pragma once

#include "StateManager.h"
#include "EventManager.h"

class MenuState : public State {
public:
	MenuState(SharedContext* l_context);
	virtual ~MenuState();

	virtual void Update(double dt);
	virtual void Render();

	void KeyCallback(CallbackData data);

private:
};