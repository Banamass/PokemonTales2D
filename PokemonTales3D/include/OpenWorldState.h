#pragma once

#include "StateManager.h"
#include "World.h"
#include "Light.h"

class OpenWorldState : public State {
public:
	OpenWorldState(SharedContext* l_context);
	~OpenWorldState();

	void Update(double dt);
	void Render();

	void Activate();
	void Desactivate();

private:
	void KeyCallback(CallbackData data);

	Cubemap skyBox;
	Camera camera;
	Light light;

	World world;
};