#pragma once

#include "StateManager.h"
#include "EventManager.h"
#include "GUI.h"

class MenuState : public State {
public:
	MenuState(SharedContext* l_context);
	virtual ~MenuState();

	virtual void Update(double dt);
	virtual void Render();

	void KeyCallback(CallbackData data);
	void MouseButtonCallback(CallbackData data);

private:
	Panel panel;

	Button* playButton;
	Button* optionsButton;
	Button* quitButton;
};