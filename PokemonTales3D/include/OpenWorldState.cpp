#include "OpenWorldState.h"
#include "Game.h"

OpenWorldState::OpenWorldState(SharedContext* l_context)
	: State(l_context), skyBox(l_context->shaderManager->GetShader("CubemapShader"), "Resources\\Skybox\\cloudy"),
	camera(l_context, StateType::OpenWorld), world(l_context), light(glm::vec3(-70.0f, 50.0f, 10.0f), l_context),
	gui(context)
{
	type = StateType::OpenWorld;

	context->eventManager->AddCallback("KeyboardOpenWorld", EventType::Key
		, &OpenWorldState::KeyCallback, this, StateType::OpenWorld);

	Camera::KeyConfig camConfig;
	camConfig.forwardK = AZERTY::O;
	camConfig.backwardK = AZERTY::L;
	camConfig.rightK = AZERTY::M;
	camConfig.leftK = AZERTY::K;

	camera.SetKeyConfig(camConfig);

	camera.SetPosition(glm::vec3(-5.0f, 5.0f, -5.0f));
	camera.SetFront(glm::vec3(1.0f, -0.7f, 1.0f));
	camera.SetIsFollowingMouse(false);

	context->openWorldGui->Subscribe("answer", &OpenWorldState::GUIAnswerNotify, this);
}
OpenWorldState::~OpenWorldState(){
	context->openWorldGui->Unsubsribe(this);
}

void OpenWorldState::Update(double dt){
	if (!gui.GetIsBlocking()) {
		camera.Update(dt);
		world.Update(dt);
	}
	gui.Update(dt);
}
void OpenWorldState::Render(){
	context->win->Draw(skyBox);

	light.Draw();
	world.Render();

	glDisable(GL_DEPTH_TEST);
	gui.Render();
	glEnable(GL_DEPTH_TEST);
}

void OpenWorldState::GUIAnswerNotify() {
	std::cout << context->openWorldGui->GetAnswer() << std::endl;
}

void OpenWorldState::KeyCallback(CallbackData data) {
	Key_Data kdata = std::get<Key_Data>(data.data);
	
	if (kdata.key == AZERTY::ESCAPE && kdata.action == GLFW_RELEASE)
		context->game->SwitchState(StateType::Menu);

	if (kdata.key == AZERTY::LEFT_CTRL && kdata.action == GLFW_RELEASE) {
		world.RotateCamera(true);
	}
	if (kdata.key == AZERTY::RIGHT_CTRL && kdata.action == GLFW_RELEASE) {
		world.RotateCamera(false);
	}
}

void OpenWorldState::Activate() {
	State::Activate();
	
	light.Activate();
	context->camera = &camera;
}

void OpenWorldState::Desactivate() {
	State::Desactivate();
	camera.SetIsFollowingMouse(false);
}
