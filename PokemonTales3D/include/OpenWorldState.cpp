#include "OpenWorldState.h"
#include "Game.h"

OpenWorldState::OpenWorldState(SharedContext* l_context)
	: State(l_context)
{
	type = StateType::OpenWorld;

	context->eventManager->AddCallback("KeyboardOpenWorld", EventType::Key
		, &OpenWorldState::KeyCallback, this, StateType::OpenWorld);
}
OpenWorldState::~OpenWorldState(){}

void OpenWorldState::Update(double dt){
	
}
void OpenWorldState::Render(){
	
}

void OpenWorldState::KeyCallback(CallbackData data) {
	Key_Data kdata = std::get<Key_Data>(data.data);
	
	if (kdata.key == AZERTY::W && kdata.action == GLFW_RELEASE)
		context->game->SwitchState(StateType::Menu);
}
