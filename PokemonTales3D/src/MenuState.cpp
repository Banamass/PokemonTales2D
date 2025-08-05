#include "MenuState.h"
#include "Game.h"

MenuState::MenuState(SharedContext* l_context)
	: State(l_context)
{
	type = StateType::Menu;

	context->eventManager->AddCallback("Menu", EventType::Key, &MenuState::KeyCallback, this, StateType::Menu);
}
MenuState:: ~MenuState() {
	context->eventManager->RemoveCallbacks("Menu");
}

void MenuState::Update(double dt) {
	
}
void MenuState::Render() {
	
}

void MenuState::KeyCallback(CallbackData data) {
	Key_Data kdata = std::get<Key_Data>(data.data);
	if (kdata.key == GLFW_KEY_Q && kdata.action == GLFW_RELEASE)
		context->game->SwitchState(StateType::Battle);
	else if (kdata.key == GLFW_KEY_ESCAPE && kdata.action == GLFW_RELEASE)
		context->win->Close();
}