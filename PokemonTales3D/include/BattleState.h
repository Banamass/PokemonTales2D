#pragma once

#include "GameSystem.h"
#include "StateManager.h"
#include "Camera.h"
#include "Light.h"

class BattleState : public State {
public:
	BattleState(SharedContext* l_context);
	virtual ~BattleState();

	virtual void Update(double dt);
	virtual void Render();

	void KeyCallback(CallbackData data);

private:
	Camera camera;

	Light light;

	Cubemap* skybox;

	GameSystem gameSystem;
	GUI gui;
};