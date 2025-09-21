#include "BattleState.h"
#include "Game.h"

/*---------------PokemonGUI---------------*/

PokemonGUI::PokemonGUI(Pokemon* l_poke, Font* l_font, SharedContext* l_context)
	: Panel(l_context->shaderManager), poke(l_poke), context(l_context)
{
	nbStepLeft = 0;
	ShaderManager* shadeMgr = context->shaderManager;
	statsBar = (PokemonStatsBar*)AddElement(new PokemonStatsBar(poke, l_font, shaderMgr));

	movesBar = (Panel*)AddElement(new Panel(shaderMgr));

	glm::vec2 moveBarPadding(30.0f, 15.0f);
	glm::vec2 moveBarFramePadding(20.0f, 20.0f);
	glm::vec2 moveBarsPos(statsBar->GetSize().x + moveBarPadding.x + moveBarFramePadding.x, -100.0f);
	for (int i = 0; i < 4; i++) {
		moveBars[i] = (PokemonMoveBar*)movesBar->AddElement(new PokemonMoveBar(l_font, shaderMgr, glm::vec2(0, 0)));
		glm::vec2 size = moveBars[i]->GetSize();
		if (i == 2)
			moveBars[i]->SetPos(moveBarsPos + glm::vec2(0, 0));
		if (i == 3)
			moveBars[i]->SetPos(moveBarsPos + glm::vec2(size.x + moveBarPadding.x, 0));
		if (i == 0)
			moveBars[i]->SetPos(moveBarsPos + glm::vec2(0, size.y + moveBarPadding.y));
		if (i == 1)
			moveBars[i]->SetPos(moveBarsPos + glm::vec2(size.x + moveBarPadding.x, size.y + moveBarPadding.y));
	}

	glm::vec2 moveBarsFrameSize(2 * moveBarFramePadding.x + moveBars[0]->GetSize().x * 2 + moveBarPadding.x,
		2 * moveBarFramePadding.y + moveBars[0]->GetSize().y * 2 + moveBarPadding.y);
	RectangleShape* moveBarsFrame = (RectangleShape*)movesBar->AddElement(
		new RectangleShape(moveBarsFrameSize, shaderMgr->GetShader("SimpleShader")), -1);
	moveBarsFrame->SetPos(moveBarsPos - moveBarFramePadding);
	moveBarsFrame->SetColor(glm::vec4(glm::vec3(1.0f), 0.5f));

	glm::vec boxSize = statsBar->GetSize();
	glm::vec2 aimedPokeStatsBarPadding(0.0f, 10.0f);
	glm::vec2 aimedPokeStatsBarPos(moveBarsPos.x + moveBarsFrameSize.x + moveBarFramePadding.x, 0.0f);
	for (int i = 0; i < MAX_AIMED_POKE; i++) {
		aimedPokeStatsBar[i] = (PokemonStatsBar*)AddElement(new PokemonStatsBar(nullptr, l_font, shaderMgr));
		aimedPokeStatsBar[i]->SetPos(aimedPokeStatsBarPos);
		aimedPokeStatsBarPos.y -= boxSize.y + aimedPokeStatsBarPadding.y;
	}

	float charSize = 35.0f;
	glm::vec2 nbStepBarSize(220.0f, 50.0f);
	nbStepBar = (Panel*)AddElement(new Panel(shadeMgr));
	RectangleShape* nbStepFrame = (RectangleShape*)AddElement(new RectangleShape(shadeMgr->GetShader("SimpleShader")));
	nbStepFrame->SetPos(glm::vec2(boxSize.x + moveBarPadding.x, boxSize.y - nbStepBarSize.y));
	nbStepFrame->SetColor(glm::vec4(glm::vec3(1.0f), 0.5f));
	nbStepFrame->SetSize(nbStepBarSize);
	nbStepText = (Text*)AddElement(new Text(l_font, "Steps left", shadeMgr->GetShader("FontShader")));
	nbStepText->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
	nbStepText->SetCharacterSize(charSize);
	nbStepText->SetPos(glm::vec2(boxSize.x + moveBarPadding.x, boxSize.y - nbStepBarSize.y) + glm::vec2(5.0f, 10.0f));
}
PokemonGUI::~PokemonGUI() {

}

void PokemonGUI::Update(double dt) {
	statsBar->Update(dt);
	for (int i = 0; i < MAX_AIMED_POKE; i++) {
		aimedPokeStatsBar[i]->Update(dt);
	}
	for (int i = 0; i < 4; i++) {
		moveBars[i]->Update(context->win);
	}
}

void PokemonGUI::Draw(glm::mat4& cameraMatrix) {
	if (poke == nullptr)
		return;
	Panel::Draw(cameraMatrix);
}

void PokemonGUI::SetPokemon(Pokemon* l_poke) {
	poke = l_poke;
	statsBar->SetPokemon(l_poke);
	if (poke == nullptr) {
		Reset();
		return;
	}
	for (int i = 0; i < 4; i++)
		moveBars[i]->SetPokemonMove(poke->GetMove(i));
}

Pokemon* PokemonGUI::GetPokemon() {
	return poke;
}

void PokemonGUI::Reset() {
	for (int i = 0; i < MAX_AIMED_POKE; i++) {
		aimedPokeStatsBar[i]->SetPokemon(nullptr);
	}
	SetSelectedMove(-1);
}

void PokemonGUI::SetSelectedMove(int i) {
	for (int j = 0; j < 4; j++)
		moveBars[j]->SetSelect(j == i);
	if (i < 0 || i > 3) {
		std::vector<Pokemon*> p;
		SetAimedPoke(p, nullptr);
	}
}

void PokemonGUI::SetNbStepsLeft(int l_nbStep) {
	if (nbStepLeft == l_nbStep)
		return;
	nbStepLeft = l_nbStep;
	std::string s = nbStepLeft < 2 ? "" : "s";
	nbStepText->SetText("Step" + s + " left : " + std::to_string(l_nbStep));
}

void PokemonGUI::SetAimedPoke(std::vector<Pokemon*>& aimedPoke, PokemonMove* move) {
	int i = 0;
	for (; i < MAX_AIMED_POKE && i < aimedPoke.size(); i++) {
		aimedPokeStatsBar[i]->SetPokemon(aimedPoke[i]);
		aimedPokeStatsBar[i]->SimulateDamages(context->gameSystem->ComputeDamages(poke, aimedPoke[i], move));
	}
	for (; i < MAX_AIMED_POKE; i++)
		aimedPokeStatsBar[i]->SetPokemon(nullptr);
}

int PokemonGUI::GetMoveClicked() {
	for (int i = 0; i < 4; i++) {
		if (moveBars[i]->GetClick())
			return i;
	}
	return -1;
}

/*---------------GUI---------------*/

BattleGUI::BattleGUI(SharedContext* l_context)
	: context(l_context),
	font(context->fontManager->RequireGetResource("Arial")),
	gameInfos(glm::vec2(175, 200), 14, glm::vec2(10, 350), l_context->shaderManager),
	gameName(l_context->shaderManager),
	cursor(glm::vec2(100.0f, 100.0f), l_context->shaderManager->GetShader("SimpleShader")),
	hoverPokeBar(nullptr, font, context->shaderManager),
	selectedPokeGUI(nullptr, font, context)
{
	context->gui = this;

	//GameInfos
	gameInfos.SetPadding(glm::vec2(10.0f, 4.0f));

	//Cursor
	cursor.SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	cursor.SetSize(glm::vec2(10.0f, 10.0f));
	cursor.SetPos(glm::vec2(Constants::WIN_WIDTH / 2, Constants::WIN_HEIGHT / 2));
	cursor.SetOrigin(Location::Middle);

	//Game Name
	Text* text = (Text*)gameName.AddElement(
		new Text(font, "PokeTales", context->shaderManager->GetShader("FontShader")));
	text->SetPos(glm::vec2(5.0f, 5.0f));
	text->SetCharacterSize(40.0f);

	glm::vec2 gameNameSize = text->GetFloatRect().size + glm::vec2(10.0f, 10.0f);
	gameName.SetPos(glm::vec2(0, Constants::WIN_HEIGHT - gameNameSize.y));

	RectangleShape* frame = (RectangleShape*)gameName.AddElement(
		new RectangleShape(gameNameSize, context->shaderManager->GetShader("SimpleShader")));
	frame->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));

	//Hove pokemons stats bar
	hoverPokeBar.SetPos(glm::vec2(0.0f, 0.0f));

	//Selected pokemon GUI
	selectedPokeGUI.SetPos(glm::vec2(225.0f, Constants::WIN_HEIGHT - hoverPokeBar.GetSize().y));
}

BattleGUI::~BattleGUI() {
	context->fontManager->ReleaseResource("Arial");
}

void BattleGUI::Update(double dt) {
	selectedPokeGUI.Update(dt);
	hoverPokeBar.Update(dt);
}

void BattleGUI::Render() {
	glDisable(GL_DEPTH_TEST);

	context->win->DrawStatic(&cursor);
	context->win->DrawStatic(&gameInfos);
	context->win->DrawStatic(&gameName);

	context->win->DrawStatic(&hoverPokeBar);
	context->win->DrawStatic(&selectedPokeGUI);

	glEnable(GL_DEPTH_TEST);
}

void BattleGUI::Reset() {
	UnsetHoverPokemon();
	UnsetSelectedPokemon();
	gameInfos.Reset();
}

void BattleGUI::SetHoverPokemon(Pokemon* poke) {
	hoverPokeBar.SetPokemon(poke);
}
void BattleGUI::UnsetHoverPokemon() {
	hoverPokeBar.SetPokemon(nullptr);
}

void BattleGUI::SetSelectedPokemon(Pokemon* poke) {
	selectedPokeGUI.SetPokemon(poke);
}
void BattleGUI::UnsetSelectedPokemon() {
	selectedPokeGUI.SetPokemon(nullptr);
}

TextField* BattleGUI::GetGameInfosField() { return &gameInfos; }
PokemonGUI* BattleGUI::GetSelectedPokemonGUI() { return &selectedPokeGUI; }

/*---------------BattleState---------------*/

BattleState::BattleState(SharedContext* l_context)
	: State(l_context),
	camera(context),
	light(glm::vec3(-70.0f, 50.0f, 10.0f), context),
	gui(context),
	inProgress(false)
{
	gameSystem = new GameSystem(context);

	type = StateType::Battle;

	camera.SetPosition(glm::vec3(-3.0f, 5.0f, -3.0f));
	camera.SetFront(glm::vec3(1.0f, -0.7f, 1.0f));

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
	if (gameSystem)
		delete gameSystem;
}

void BattleState::Update(double dt){
	camera.Update(dt);
	gui.Update(dt);
	gameSystem->Update(dt);
}
void BattleState::Render(){
	context->win->Draw(skybox);

	gameSystem->Render();
	gui.Render();
	light.Draw(context->win);
}

void BattleState::Activate() {
	State::Activate();
	if (!inProgress) {
		std::cout << "Start a new game !" << std::endl;
		gameSystem->StartBattle();
		inProgress = true;
	}
}

void BattleState::Restart() {
	if (isActivated) {
		std::cout << "Warning : cannot restart a battle while it's running" << std::endl;
		return;
	}
	if (!inProgress)
		return;
	if (gameSystem)
		delete gameSystem;
	gameSystem = new GameSystem(context);
	inProgress = false;
	gui.Reset();
}

void BattleState::KeyCallback(CallbackData data) {
	Key_Data kdata = std::get<Key_Data>(data.data);
	if (kdata.key == GLFW_KEY_ESCAPE && kdata.action == GLFW_RELEASE)
		context->game->SwitchState(StateType::Menu);
}

BattleData BattleState::GetBattleData() {
	BattleData data;
	data.battleInProgress = inProgress;
	return data;
}