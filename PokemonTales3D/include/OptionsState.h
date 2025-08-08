#pragma once

#include "StateManager.h"
#include "EventManager.h"
#include "GUI.h"
#include <vector>

class OptionsState : public State {
public:
	OptionsState(SharedContext* l_context);
	virtual ~OptionsState();

	virtual void Update(double dt);
	virtual void Render();

	void KeyCallback(CallbackData data);
	void MouseButtonCallback(CallbackData data);

private:
	std::vector<SelectBox*> pokeSelectPlayer1;
	std::vector<SelectBox*> pokeSelectPlayer2;
};