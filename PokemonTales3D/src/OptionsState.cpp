#include "OptionsState.h"
#include "Game.h"

/*--------------PlayerOptionsGUI--------------*/

PlayerOptionsGUI::PlayerOptionsGUI(SharedContext* l_context, std::string l_playerTag)
	: Panel(shaderMgr), activated(true), playerTag(l_playerTag), otherPlayerOpt(nullptr)
{
	defaultSelection = "Select a Pokemon";
	noneField = "No Pokemon";

	Font* font = l_context->fontManager->GetResource("Arial");

	playerTagText = (Text*)AddElement(
		new Text(font, playerTag + " :", l_context->shaderManager->GetShader("FontShader")));
	playerTagText->SetCharacterSize(30.0f);
	playerTagText->SetColor(glm::vec3(1.0f));
	playerTagText->SetOrigin(Location::MiddleLeft);

	playerColorSelection = (ColorSelection*)AddElement(
		new ColorSelection(l_context->shaderManager, Orientation::Hozirontal));
	playerColorSelection->Subscribe("Color", &PlayerOptionsGUI::ColorSelectionNotify, this);
	playerColorSelection->SetSize(glm::vec2(60.0f, 60.0f));
	playerColorSelection->SetPos(glm::vec2(500.0f, 0.0f));
	playerColorSelection->SetOrigin(Location::MiddleLeft);

	textSelectColor = (Text*)AddElement(
		new Text(font, "Select a color : ", l_context->shaderManager->GetShader("FontShader")));
	textSelectColor->SetCharacterSize(20.0f);
	textSelectColor->SetColor(glm::vec3(1.0f));
	textSelectColor->SetPos(glm::vec2(330.0f, 0.0f));
	textSelectColor->SetOrigin(Location::MiddleLeft);

	glm::vec2 selectSize(200.0f, 50.0f);
	float space = 40.0f;
	float yPos = selectSize.y * -2;

	std::vector<std::string> selectionFields;
	const std::map<int, PokemonData>& data = l_context->gameData->GetAllPokemonData();
	for (auto& d : data) {
		selectionFields.push_back(d.second.name);
	}
	selectionFields.push_back(noneField);

	for (int i = 0; i < Constants::NB_POKEMON_BY_PLAYER; i++) {
		pokeSelection.push_back((SelectBox*)AddElement(new SelectBox(
			font, l_context->shaderManager,
			selectionFields, selectSize, 3, defaultSelection
		)));
		pokeSelection[i]->SetPos(glm::vec2((selectSize.x + space) * i, yPos));
	}
}
PlayerOptionsGUI::~PlayerOptionsGUI() {
	playerColorSelection->Unsubsribe(this);
	if(playerColorSelection != nullptr)
		playerColorSelection->Unsubsribe(playerColorSelection);
}

void PlayerOptionsGUI::SetOtherPlayerOption(PlayerOptionsGUI* l_otherPlayerOpt) {
	otherPlayerOpt = l_otherPlayerOpt;
	playerColorSelection->Subscribe("Color", &PlayerOptionsGUI::ColorOtherPlayerNotify, otherPlayerOpt);
}

void PlayerOptionsGUI::Update(Window* win) {
	if (!activated)
		return;
	for (auto& s : pokeSelection)
		s->Update(win);
	playerColorSelection->Update(win);
}

void PlayerOptionsGUI::Scroll(double xoffset, double yoffset) {
	for (auto& box : pokeSelection) {
		if(box->GetIsInSelection())
			box->Scroll(xoffset, yoffset);
	}
}

void PlayerOptionsGUI::SetActivated(bool b) {
	activated = b;
}

void PlayerOptionsGUI::AddPlayerColor(glm::vec4 color) {
	playerColorSelection->AddColor(color);
	playerColorSelection->SetOrigin(Location::MiddleLeft);
}
void PlayerOptionsGUI::SetSelectedPlayerColor(glm::vec4 color) {
	playerColorSelection->SetSelectedColor(color);
}

void PlayerOptionsGUI::Reset() {
	for (auto& s : pokeSelection) {
		s->ResetSelectedField();
	}
}

std::vector<std::string> PlayerOptionsGUI::GetSelectedPokemon() {
	std::vector<std::string> res;
	for (auto& s : pokeSelection) {
		std::string select = s->GetSelectedField();
		if (select != defaultSelection && select != noneField)
			res.push_back(select);
	}
	return res;
}

glm::vec4 PlayerOptionsGUI::GetPlayerColor() {
	return playerColorSelection->GetSelectedColor();
}

void PlayerOptionsGUI::ColorSelectionNotify() {
	playerTagText->SetColor(playerColorSelection->GetSelectedColor());
}
void PlayerOptionsGUI::ColorOtherPlayerNotify() {
	std::vector<glm::vec4> colors;
	colors.push_back(otherPlayerOpt->GetPlayerColor());
	playerColorSelection->SetDesactivatedColors(colors);
}

/*--------------OptionsState--------------*/

OptionsState::OptionsState(SharedContext* l_context)
	: State(l_context), player1Options(l_context, "Player1")
	, player2Options(l_context, "Player2"),
	generalButtons(l_context->shaderManager)
{
	type = StateType::Options;

	context->eventManager->AddCallback("OptionsKey", EventType::Key, &OptionsState::KeyCallback, this, StateType::Options);
	context->eventManager->AddCallback("OptionsScroll", EventType::Scroll, &OptionsState::ScrollCallback, this, StateType::Options);
	
	//Setup color selection
	{
		player1Options.SetOtherPlayerOption(&player2Options);
		player2Options.SetOtherPlayerOption(&player1Options);

		player1Options.AddPlayerColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		player1Options.AddPlayerColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		player1Options.AddPlayerColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

		player2Options.AddPlayerColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		player2Options.AddPlayerColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		player2Options.AddPlayerColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

		player1Options.SetSelectedPlayerColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		player2Options.SetSelectedPlayerColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}

	glm::vec2 select1Pos(10.0f, 400.0f);
	glm::vec2 select2Pos(10.0f, 100.0f);

	player1Options.SetOrigin(Location::BottomLeft);
	player2Options.SetOrigin(Location::BottomLeft);

	player1Options.SetPos(select1Pos);
	player2Options.SetPos(select2Pos);

	battleState = new Text(context->fontManager->GetResource("Arial"), "No battle in progress"
		, context->shaderManager->GetShader("FontShader"));
	battleState->SetCharacterSize(20.0f);
	battleState->SetColor(glm::vec4(1.0f));
	battleState->SetPos(glm::vec2(Constants::WIN_WIDTH - 220.0f, Constants::WIN_HEIGHT - 40.0f));

	SetUpGeneralButtons();
}
OptionsState::~OptionsState(){
	context->eventManager->RemoveCallbacks("OptionsKey");
	context->eventManager->RemoveCallbacks("OptionsScroll");

	delete battleState;
}

void OptionsState::SetUpGeneralButtons() {
	generalButtons.SetPos(glm::ivec2(Constants::WIN_WIDTH - 125.0f, 25.0f));
	
	restartButton = (Button*)generalButtons.AddElement(new Button
	(context->fontManager->GetResource("Arial"), context->shaderManager, glm::ivec2(0, 0)));
	restartButton->SetText("Restart");
	restartButton->SetSize(glm::ivec2(100.0f, 50.0f));
	restartButton->SetPos(glm::ivec2(0.0f, 75.0f));
	restartButton->SetCharacterSize(20.0f);
	restartButton->SetTextColor(glm::vec4(glm::vec3(0.0f), 1.0f));

	quitButton = (Button*)generalButtons.AddElement(new Button
	(context->fontManager->GetResource("Arial"), context->shaderManager, glm::ivec2(0, 0)));
	quitButton->SetText("Quit");
	quitButton->SetSize(glm::ivec2(100.0f, 50.0f));
	quitButton->SetPos(glm::ivec2(0.0f, 0.0f));
	quitButton->SetCharacterSize(20.0f);
	quitButton->SetTextColor(glm::vec4(glm::vec3(0.0f), 1.0f));

	okButton = (Button*)generalButtons.AddElement(new Button
	(context->fontManager->GetResource("Arial"), context->shaderManager, glm::ivec2(0, 0)));
	okButton->SetText("Ok");
	okButton->SetSize(glm::ivec2(100.0f, 50.0f));
	okButton->SetPos(glm::ivec2(0.0f, 150.0f));
	okButton->SetCharacterSize(20.0f);
	okButton->SetTextColor(glm::vec4(glm::vec3(0.0f), 1.0f));
}

void OptionsState::Update(double dt){
	player1Options.Update(context->win);
	player2Options.Update(context->win);
	restartButton->Update(context->win);
	quitButton->Update(context->win);
	okButton->Update(context->win);

	if (restartButton->GetClick()) {
		Restart();
	}
	if (quitButton->GetClick()) {
		player1Options.Reset();
		player2Options.Reset();
		Quit();
	}
	if (okButton->GetClick()) {
		Quit();
	}
}
void OptionsState::Render(){
	glDisable(GL_DEPTH_TEST);

	context->win->DrawStatic(&player1Options);
	context->win->DrawStatic(&player2Options);
	context->win->DrawStatic(&generalButtons);
	context->win->DrawStatic(battleState);

	glEnable(GL_DEPTH_TEST);
}

void OptionsState::Activate() {
	BattleData data = ((BattleState*)context->stateManager->GetState(StateType::Battle))->GetBattleData();
	SetBattleState(data.battleInProgress);
}

void OptionsState::Quit() {
	context->game->SwitchState(StateType::Menu);
}

void OptionsState::Restart() {
	((BattleState*)context->stateManager->GetState(StateType::Battle))->Restart();
	player1Options.Reset();
	player2Options.Reset();
	SetBattleState(false);
}

void OptionsState::SetBattleState(bool isInProgress) {
	if (isInProgress) {
		player1Options.SetActivated(false);
		player2Options.SetActivated(false);
		battleState->SetText("Battle In Progress");
		battleState->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else {
		player1Options.SetActivated(true);
		player2Options.SetActivated(true);
		battleState->SetText("No Battle In Progress");
		battleState->SetColor(glm::vec4(1.0f));
	}
}

void OptionsState::KeyCallback(CallbackData data){
	Key_Data kdata = std::get<Key_Data>(data.data);
	if (kdata.key == GLFW_KEY_ESCAPE && kdata.action == GLFW_RELEASE)
		Quit();
}
void OptionsState::MouseButtonCallback(CallbackData data){}
void OptionsState::ScrollCallback(CallbackData data) {
	Scroll_Data sdata = std::get<Scroll_Data>(data.data);
	player1Options.Scroll(sdata.xoffset, sdata.yoffset);
	player2Options.Scroll(sdata.xoffset, sdata.yoffset);
}

const OptionsData& OptionsState::GetOptionsData() {
	std::vector<std::string> p1selection = player1Options.GetSelectedPokemon();
	std::vector<std::string> p2selection = player2Options.GetSelectedPokemon();

	data.pokeNamePlayer1.clear();
	data.pokeNamePlayer2.clear();

	for (auto& p : p1selection) {
		const PokemonData* pokeData = context->gameData->GetPokemonData(p);
		if (pokeData)
			data.pokeNamePlayer1.push_back(pokeData);
	}
	for (auto& p : p2selection) {
		const PokemonData* pokeData = context->gameData->GetPokemonData(p);
		if (pokeData)
			data.pokeNamePlayer2.push_back(pokeData);
	}

	data.player1Color = player1Options.GetPlayerColor();
	data.player2Color = player2Options.GetPlayerColor();

	return data;
}