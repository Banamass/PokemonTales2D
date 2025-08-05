#include "BattleState.h"
#include "Game.h"

BattleState::BattleState(SharedContext* l_context)
	: State(l_context),
	camera(context),
	gameSystem(context),
	light(glm::vec3(-70.0f, 50.0f, 10.0f), context),
	gui(context)
{
	type = StateType::Battle;

	camera.SetPosition(glm::vec3(-3.0f, 5.0f, -3.0f));

	std::string dir = "Resources\\Skybox";
	std::vector<std::string> faces
	{
			"right.jpg",
			"left.jpg",
			"top.jpg",
			"bottom.jpg",
			"front.jpg",
			"back.jpg"
	};
	skybox = new Cubemap(context->shaderManager->GetShader("CubemapShader"), faces, dir);

	context->eventManager->AddCallback("Battle", EventType::Key, &BattleState::KeyCallback, this, StateType::Battle);
}
BattleState:: ~BattleState(){
	context->eventManager->RemoveCallbacks("Battle");
	delete skybox;
}

void BattleState::Update(double dt){
	camera.Update(dt);
	gameSystem.Update(dt);
	gui.Update(dt);
}
void BattleState::Render(){
	context->win->Draw(skybox);

	gameSystem.Render();
	gui.Render();
	light.Draw(context->win);
}

void BattleState::KeyCallback(CallbackData data) {
	Key_Data kdata = std::get<Key_Data>(data.data);
	if (kdata.key == GLFW_KEY_ESCAPE && kdata.action == GLFW_RELEASE)
		context->game->SwitchState(StateType::Menu);
}