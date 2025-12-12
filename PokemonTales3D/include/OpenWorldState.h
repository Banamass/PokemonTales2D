#pragma once

#include "StateManager.h"
#include "World.h"
#include "Light.h"
#include "OpenWorldGUI.h"

class OpenWorldState : public State {
public:
	OpenWorldState(SharedContext* l_context);
	virtual ~OpenWorldState();

	void Update(double dt);
	void Render();

	void Activate();
	void Desactivate();

private:
	void GUIAnswerNotify();

	void KeyCallback(CallbackData data);

	OpenWorldGUI gui;

	Cubemap skyBox;
	Camera camera;
	Light light;

	World world;
};