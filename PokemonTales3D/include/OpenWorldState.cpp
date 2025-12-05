#include "OpenWorldState.h"
#include "Game.h"

OpenWorldState::OpenWorldState(SharedContext* l_context)
	: State(l_context), skyBox(l_context->shaderManager->GetShader("CubemapShader"), "Resources\\Skybox\\cloudy"),
	camera(l_context, StateType::OpenWorld), world(l_context), light(glm::vec3(-70.0f, 50.0f, 10.0f), l_context)
{
	type = StateType::OpenWorld;

	context->eventManager->AddCallback("KeyboardOpenWorld", EventType::Key
		, &OpenWorldState::KeyCallback, this, StateType::OpenWorld);
}
OpenWorldState::~OpenWorldState(){}

void OpenWorldState::Update(double dt){
	camera.Update(dt);

	world.Update(dt);
}
void OpenWorldState::Render(){
	context->win->Draw(skyBox);

	light.Draw();
	world.Render();
}

void OpenWorldState::KeyCallback(CallbackData data) {
	Key_Data kdata = std::get<Key_Data>(data.data);
	
	if (kdata.key == AZERTY::ESCAPE && kdata.action == GLFW_RELEASE)
		context->game->SwitchState(StateType::Menu);
}

void OpenWorldState::Activate() {
	State::Activate();
	
	light.Activate();
	context->camera = &camera;
	camera.SetIsFollowingMouse(true);
}

void OpenWorldState::Desactivate() {
	State::Desactivate();
	camera.SetIsFollowingMouse(false);
}
