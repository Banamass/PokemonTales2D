#include "MenuState.h"
#include "Game.h"

MenuState::MenuState(SharedContext* l_context)
	: State(l_context), panel(context->shaderManager)
{
	type = StateType::Menu;

	context->eventManager->AddCallback("Menu", EventType::Key, &MenuState::KeyCallback, this, StateType::Menu);
	context->eventManager->AddCallback("Menu", EventType::MouseButton, &MenuState::MouseButtonCallback, this, StateType::Menu);

	glm::vec2 pos(Constants::WIN_WIDTH / 2.0f, Constants::WIN_HEIGHT - 100.0f);
	panel.SetPos(glm::vec2(pos));

	glm::vec2 tSize(400.0f, 120.0f);
	float charSize = 70.0f;

	Text* t = (Text*)panel.AddElement(
		new Text(context->fontManager->GetResource("Arial"), "PokeTales", context->shaderManager->GetShader("FontShader")));
	t->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	t->SetCharacterSize(charSize);
	t->SetPos(-t->GetFloatRect().size / 2.0f);

	RectangleShape* tFrame = (RectangleShape*)panel.AddElement(
		new RectangleShape(tSize, context->shaderManager->GetShader("SimpleShader")), -1);
	tFrame->SetOrigin(Location::Middle);
	tFrame->SetColor(glm::vec4(1.0f));

	glm::vec4 bColor(1.0f);
	glm::vec4 bhColor(0.8f);
	glm::vec4 bpColor(0.5f);
	glm::vec4 tbColor(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec2 bSize(200.0f, 70.0f);
	float padding = 20.0f;
	charSize = 30.0f;

	glm::vec2 bPos(0, -tSize.y - padding);
	
	playButton = (Button*)panel.AddElement(
		new Button(context->fontManager->GetResource("Arial"), context->shaderManager, bPos));
	playButton->SetSize(bSize);
	playButton->SetText("Play");
	playButton->SetFrameColor(bColor);
	playButton->SetFrameHoverColor(bhColor);
	playButton->SetFramePressColor(bpColor);
	playButton->SetTextColor(tbColor);
	playButton->SetCharacterSize(charSize);
	playButton->SetOrigin(Location::Middle);

	bPos += glm::vec2(0, -bSize.y - padding);

	optionsButton = (Button*)panel.AddElement(
		new Button(context->fontManager->GetResource("Arial"), context->shaderManager, bPos));
	optionsButton->SetSize(bSize);
	optionsButton->SetText("Options");
	optionsButton->SetFrameColor(bColor);
	optionsButton->SetFrameHoverColor(bhColor);
	optionsButton->SetFramePressColor(bpColor);
	optionsButton->SetTextColor(tbColor);
	optionsButton->SetCharacterSize(charSize);
	optionsButton->SetOrigin(Location::Middle);

	bPos += glm::vec2(0, -bSize.y - padding);

	quitButton = (Button*)panel.AddElement(
		new Button(context->fontManager->GetResource("Arial"), context->shaderManager, bPos));
	quitButton->SetSize(bSize);
	quitButton->SetText("Quit");
	quitButton->SetFrameColor(bColor);
	quitButton->SetFrameHoverColor(bhColor);
	quitButton->SetFramePressColor(bpColor);
	quitButton->SetTextColor(tbColor);
	quitButton->SetCharacterSize(charSize);
	quitButton->SetOrigin(Location::Middle);
}
MenuState:: ~MenuState() {
	context->eventManager->RemoveCallbacks("Menu");
}

void MenuState::Update(double dt) {
	playButton->Update(context->win);
	optionsButton->Update(context->win);
	quitButton->Update(context->win);

	if (playButton->GetClick())
		context->game->SwitchState(StateType::Battle);
	else if (optionsButton->GetClick())
		context->game->SwitchState(StateType::Options);
	else if (quitButton->GetClick())
		context->win->Close();
}
void MenuState::Render() {
	glDisable(GL_DEPTH_TEST);

	context->win->DrawStatic(&panel);

	glEnable(GL_DEPTH_TEST);
}

void MenuState::KeyCallback(CallbackData data) {
	Key_Data kdata = std::get<Key_Data>(data.data);
	if (kdata.key == GLFW_KEY_Q && kdata.action == GLFW_RELEASE)
		context->game->SwitchState(StateType::Battle);
	else if (kdata.key == GLFW_KEY_ESCAPE && kdata.action == GLFW_RELEASE)
		context->win->Close();
}

void MenuState::MouseButtonCallback(CallbackData data) {
	MouseButton_Data mdata = std::get<MouseButton_Data>(data.data);
	
}