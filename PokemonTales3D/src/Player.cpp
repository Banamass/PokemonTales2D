#include "Player.h"
#include "GameSystem.h"
#include "Camera.h"
#include "BattleState.h"

/*-----------------------------Player-----------------------------*/

Player::Player(SharedContext* l_context) 
	: APlayer(l_context), state(nullptr), nextState(nullptr)
{
	if (!context->modelManager->RequireResource("Box")) {
		std::cout << "Box model not found" << std::endl;
	}
	boxModel = context->modelManager->GetResource("Box");

	Setup();

	context->camera->SetIsFollowingMouse(false);
}
Player::~Player() {
	context->modelManager->ReleaseResource("Box");
	context->eventManager->RemoveCallbacks("PlayerKey");
	context->eventManager->RemoveCallbacks("PlayerMouseButton");
}

void Player::Setup() {
	context->eventManager->AddCallback("PlayerKey", EventType::Key, &Player::KeyCallback, this, StateType::Battle);
	context->eventManager->AddCallback("PlayerMouseButton", EventType::MouseButton, &Player::MouseButtonCallback, this, StateType::Battle);
}

void Player::KeyCallback(CallbackData data) {
	if (isPlaying) {
		Key_Data kdata = std::get<Key_Data>(data.data);
		if (kdata.key == GLFW_KEY_1 && kdata.action == GLFW_PRESS) {
			context->gui->GetGameInfosField()->AddMessage("Fin de tour");
			EndTurn();
			return;
		}
		else if (kdata.key == GLFW_KEY_Q && kdata.action == GLFW_PRESS)
			context->camera->SetIsFollowingMouse(true);
		else if (kdata.key == GLFW_KEY_Q && kdata.action == GLFW_RELEASE)
			context->camera->SetIsFollowingMouse(false);
		if(state != nullptr)
			state->KeyCallback(kdata);
	}
}

void Player::MouseButtonCallback(CallbackData data) {
	if (isPlaying) {
		MouseButton_Data kdata = std::get<MouseButton_Data>(data.data);
		if (state != nullptr)
			state->MouseButtonCallback(kdata);
	}
}

void Player::PlayTurn() {
	APlayer::PlayTurn();
	SwitchState(new DefaultState(this));
}

void Player::Update(double dt) {
	if (nextState != nullptr) {
		PType type = nextState->GetType();
		if (type != PType::None) {
			if (state != nullptr) {
				delete state;
			}
			state = nextState;
			nextState = nullptr;
		}
	}
	APlayer::Update(dt);
	if(state != nullptr)
		state->Update(dt);
}

void Player::Render() {
	APlayer::Render();
	if(state != nullptr && isPlaying)
		state->Render();
}

void Player::EndTurn() { 
	APlayer::EndTurn();
	context->gui->SetSelectedPokemon(nullptr);
}

void Player::SwitchState(State* newState) {
	nextState = newState;
}

/*-----------------------------State-----------------------------*/

Player::State::State(Player* l_player) : 
	player(l_player), type(PType::None),
	cursorDrawable(player->boxModel, player->context->shaderManager->GetShader("ModelShader")),
	selectedPokeAreaDrawable(player->boxModel, player->context->shaderManager->GetShader("ModelShader")),
	cursor(player->context->board, glm::ivec2(1, 1)),
	selectedPokeArea(player->context->board, glm::ivec2(0,0))
{
	Drawable::Material mat;

	glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 0.7f);
	mat.ambient = 0.3f * color;
	mat.diffuse = color;
	selectedPokeAreaDrawable.SetMaterial(mat);
}
Player::State::~State() {
	player->context->gui->GetSelectedPokemonGUI()->Unsubsribe(this);
}

void Player::State::Render() {
	RenderSelectedPokeArea();
}

void Player::State::UpdateCursor() {
	Board* board = player->context->board;
	cursor.Update(board->GetMousePos());
}
void Player::State::RenderCursor() {
	for (Box* box : cursor.GetBoxes()) {
		glm::ivec2 pos = box->GetPos();
		cursorDrawable.SetPosition(glm::ivec3(
			pos.x * Constants::BOX_SIZE,
			0.0f,
			pos.y * Constants::BOX_SIZE
		));
		player->context->win->Draw(cursorDrawable);
	}
}
void Player::State::UpdateSelectedPokeArea(Pokemon* poke) {
	if (poke == nullptr) {
		selectedPokeArea.SetSize(glm::ivec2(0, 0));
		return;
	}
	selectedPokeArea.SetSize(player->GetPokemonRotateSize(poke));
	Board* board = player->context->board;
	selectedPokeArea.Update(board->GetPokemonPosition(poke));
}
void Player::State::RenderSelectedPokeArea() {
	for (Box* box : selectedPokeArea.GetBoxes()) {
		glm::ivec2 pos = box->GetPos();
		selectedPokeAreaDrawable.SetPosition(glm::ivec3(
			pos.x * Constants::BOX_SIZE,
			0.0f,
			pos.y * Constants::BOX_SIZE
		));
		player->context->win->Draw(selectedPokeAreaDrawable);
	}
}

/*-----------------------------DefaultState-----------------------------*/

Player::DefaultState::DefaultState(Player* l_player) 
	: State(l_player)
		{
	type = PType::Default;

	Drawable::Material mat;
	mat.ambient = 0.3f * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	mat.diffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	mat.specular = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	mat.shininess = 32.0f;
	cursorDrawable.SetMaterial(mat);

	UpdateSelectedPokeArea(nullptr);
}

void Player::DefaultState::KeyCallback(Key_Data& data) {
	
}

void Player::DefaultState::MouseButtonCallback(MouseButton_Data& data) {
	if (data.button == GLFW_MOUSE_BUTTON_LEFT && data.action == GLFW_RELEASE) {
		Select();
		return;
	}
}

void Player::DefaultState::Update(double dt) {
	State::Update(dt);
	UpdateCursor();
}

void Player::DefaultState::Render(){
	State::Render();
	RenderCursor();
}

void Player::DefaultState::Select() {
	Pokemon* poke = player->context->board->GetPokemonFromPos(cursor.GetIntRect().pos);
	if (poke == nullptr || poke->IsKO() || poke->GetTrainer() != player)
		return;
	player->context->gui->GetGameInfosField()->AddMessage("Select " + poke->GetName());
	player->SwitchState(new PokeSelectedState(player, poke));
}

/*-----------------------------PokeSelectedState-----------------------------*/

Player::PokeSelectedState::PokeSelectedState(Player* l_player, Pokemon* l_selectedPokemon)
	: State(l_player), selectedPokemon(l_selectedPokemon) {
	type = PType::PokeSelected;
	player->context->gui->SetSelectedPokemon(selectedPokemon);
	player->context->gui->GetSelectedPokemonGUI()->SetNbStepsLeft(player->pokemonState[l_selectedPokemon].nbStepLeft);
	UpdateSelectedPokeArea(selectedPokemon);

	PokemonGUI* gui = player->context->gui->GetSelectedPokemonGUI();
	gui->Subscribe("Step", &Player::PokeSelectedState::Move, this);
}

void Player::PokeSelectedState::Update(double dt) {
	State::Update(dt);

	BattleGUI* gui = player->context->gui;

	if (selectedPokemon->IsKO()) {
		gui->GetGameInfosField()->AddMessage("Selected Pokemon " + selectedPokemon->GetName() + " died.");
		player->EndTurn();
		return;
	}
	int moveId = gui->GetSelectedPokemonGUI()->GetMoveClicked();
	if (moveId != -1){
		Attack(moveId);
		return;
	}
}

void Player::PokeSelectedState::KeyCallback(Key_Data& data) {
	if (data.key == GLFW_KEY_2 && data.action == GLFW_RELEASE)
		Unselect();
	else if (data.key == GLFW_KEY_3 && data.action == GLFW_RELEASE)
		Move();
	else if (data.key == GLFW_KEY_KP_1 && data.action == GLFW_RELEASE)
		Attack(0);
	else if (data.key == GLFW_KEY_KP_2 && data.action == GLFW_RELEASE)
		Attack(1);
	else if (data.key == GLFW_KEY_KP_3 && data.action == GLFW_RELEASE)
		Attack(2);
	else if (data.key == GLFW_KEY_KP_4 && data.action == GLFW_RELEASE)
		Attack(3);
	else if (data.key == GLFW_KEY_R && data.action == GLFW_RELEASE)
		RotatePokemon();
}

void Player::PokeSelectedState::RotatePokemon() {
	player->pokemonState[selectedPokemon].rotate = !player->pokemonState[selectedPokemon].rotate;
	selectedPokeArea.Rotate();
}

void Player::PokeSelectedState::MouseButtonCallback(MouseButton_Data& data) {
	if (data.button == GLFW_MOUSE_BUTTON_RIGHT && data.action == GLFW_RELEASE) {
		Unselect();
	}
}

void Player::PokeSelectedState::Unselect() {
	if (player->pokemonState[selectedPokemon].lock) {
		player->context->gui->GetGameInfosField()->AddMessage("Impossible to unselect");
		return;
	}
	player->context->gui->GetGameInfosField()->AddMessage("Unselect " + selectedPokemon->GetName());
	player->context->gui->SetSelectedPokemon(nullptr);
	player->SwitchState(new DefaultState(player));
}

void Player::PokeSelectedState::Move() {
	if (player->pokemonState[selectedPokemon].nbStepLeft == 0) {
		player->context->gui->GetGameInfosField()->AddMessage("No step left");
		return;
	}
	player->SwitchState(new PokeMoveState(player, selectedPokemon));
}

void Player::PokeSelectedState::Attack(int moveId) {
	if (player->pokemonState[selectedPokemon].nbMove > 0) {
		player->context->gui->GetGameInfosField()->AddMessage("Max move in a turn reached");
		return;
	}
	if (selectedPokemon->GetMove(moveId) == nullptr)
		return;
	player->SwitchState(new PokeAttackState(player, selectedPokemon, moveId));
}

/*-----------------------------PokeMoveState-----------------------------*/

Player::PokeMoveState::PokeMoveState(Player* l_player, Pokemon* l_selectedPokemon)
	: State(l_player), selectedPokemon(l_selectedPokemon)
	, moveArea(player->context->board, l_selectedPokemon, player->pokemonState[l_selectedPokemon].nbStepLeft),
	moveBox(player->boxModel, player->context->shaderManager->GetShader("ModelShader"))
{
	type = PType::PokeMove;

	Drawable::Material mat;
	glm::vec3 color(0.0f, 0.0f, 0.7f);
	mat.ambient = 0.3f * color;
	mat.diffuse = color;
	mat.specular = glm::vec4(1.0);
	mat.shininess = 32.0f;
	cursorDrawable.SetMaterial(mat);

	color = glm::vec3(0.0f, 0.0f, 1.0f);
	mat.ambient = 0.3f * color;
	mat.diffuse = color;
	mat.alpha = 0.2f;
	moveBox.SetMaterial(mat);
	float scale = 0.95f;
	moveBox.Scale(glm::vec3(scale, scale, scale));

	moveArea.Update(player->context->board->GetPokemonPosition(selectedPokemon));
	
	UpdateSelectedPokeArea(selectedPokemon);
}

void Player::PokeMoveState::KeyCallback(Key_Data& data) {
	if (data.key == GLFW_KEY_2 && data.action == GLFW_RELEASE)
		Unmove();
}
void Player::PokeMoveState::MouseButtonCallback(MouseButton_Data& data) {
	if (data.button == GLFW_MOUSE_BUTTON_LEFT && data.action == GLFW_RELEASE)
		Move();
	if (data.button == GLFW_MOUSE_BUTTON_RIGHT && data.action == GLFW_RELEASE) {
		Unmove();
	}
}

void Player::PokeMoveState::Render() {
	State::Render();
	for (Box*& box : moveArea.GetBoxes()) {
		glm::ivec2 pos = box->GetPos();
		moveBox.SetPosition(glm::ivec3(
			pos.x * Constants::BOX_SIZE,
			0.0f,
			pos.y * Constants::BOX_SIZE
		));
		player->context->win->Draw(moveBox);
	}
	RenderCursor();
}
void Player::PokeMoveState::Update(double dt) {
	State::Update(dt);
	if (selectedPokemon->IsKO()) {
		player->context->gui->GetGameInfosField()->AddMessage("Selected Pokemon " + selectedPokemon->GetName() + " died.");
		player->EndTurn();
		return;
	}

	BattleGUI* gui = player->context->gui;
	int moveId = gui->GetSelectedPokemonGUI()->GetMoveClicked();
	if (moveId != -1) {
		Attack(moveId);
		return;
	}

	UpdateCursor();
}

void Player::PokeMoveState::Attack(int moveId) {
	if (player->pokemonState[selectedPokemon].nbMove > 0) {
		player->context->gui->GetGameInfosField()->AddMessage("Max move in a turn reached");
		return;
	}
	if (selectedPokemon->GetMove(moveId) == nullptr)
		return;
	player->SwitchState(new PokeAttackState(player, selectedPokemon, moveId));
}

void Player::PokeMoveState::Unmove() {
	player->context->gui->GetGameInfosField()->AddMessage("Unmove " + selectedPokemon->GetName());
	player->SwitchState(new PokeSelectedState(player, selectedPokemon));
}

void Player::PokeMoveState::Move() {
	glm::ivec2 movePos = cursor.GetIntRect().pos;
	int dist = moveArea.Distance(movePos);
	if (dist <= 0)
		return;
	bool hasMoved = player->context->board->SetPokemonPos(selectedPokemon, movePos);
	if (!hasMoved)
		return;
	player->context->gui->GetGameInfosField()->AddMessage("Move " + selectedPokemon->GetName());
	player->pokemonState[selectedPokemon].nbStepLeft -= dist;
	player->pokemonState[selectedPokemon].lock = true;
	player->SwitchState(new PokeSelectedState(player, selectedPokemon));
}

/*-----------------------------PokeAttackState-----------------------------*/
Player::PokeAttackState::PokeAttackState(Player* l_player, Pokemon* l_selectedPokemon, int moveId)
	: State(l_player), selectedPokemon(l_selectedPokemon),
	rangeArea(player->context->board, glm::ivec2(0,0)), rangeBox(player->boxModel, player->context->shaderManager->GetShader("ModelShader"))
{
	type = PType::PokeAttack;

	Drawable::Material mat;
	glm::vec3 color(0.8f, 0.0f, 0.0f);
	mat.ambient = 0.3f * color;
	mat.diffuse = color;
	mat.specular = glm::vec4(1.0);
	mat.shininess = 32.0f;
	cursorDrawable.SetMaterial(mat);
	
	color = glm::vec3(1.0f, 0.0f, 0.0f);
	mat.ambient = 0.3f * color;
	mat.diffuse = color;
	mat.alpha = 0.2f;
	rangeBox.SetMaterial(mat);
	float scale = 0.95f;
	rangeBox.Scale(glm::vec3(scale, scale, scale));

	SetMove(moveId);

	UpdateSelectedPokeArea(selectedPokemon);

	player->context->gui->GetSelectedPokemonGUI()->Subscribe("Step", &PokeAttackState::Move, this);
}

void Player::PokeAttackState::SetMove(int id) {
	if (selectedPokemon->GetMove(id) == nullptr)
		return;
	move = selectedPokemon->GetMove(id);
	player->context->gui->GetSelectedPokemonGUI()->SetSelectedMove(id);
	cursor.SetSize(move->data->hitZone);
	rangeArea.SetSize(glm::ivec2(move->data->range*2) + selectedPokemon->GetSize());
	Board* board = player->context->board;
	rangeArea.Update(board->GetPokemonPosition(selectedPokemon));
}

void Player::PokeAttackState::KeyCallback(Key_Data& data) {
	if (data.key == GLFW_KEY_2 && data.action == GLFW_RELEASE)
		Unattack();
	else if (data.key == GLFW_KEY_KP_1 && data.action == GLFW_RELEASE)
		SetMove(0);
	else if (data.key == GLFW_KEY_KP_2 && data.action == GLFW_RELEASE)
		SetMove(1);
	else if (data.key == GLFW_KEY_KP_3 && data.action == GLFW_RELEASE)
		SetMove(2);
	else if (data.key == GLFW_KEY_KP_4 && data.action == GLFW_RELEASE)
		SetMove(3);
	else if (data.key == GLFW_KEY_R && data.action == GLFW_RELEASE)
		cursor.Rotate();
}

void Player::PokeAttackState::MouseButtonCallback(MouseButton_Data& data) {
	if (data.button == GLFW_MOUSE_BUTTON_LEFT && data.action == GLFW_RELEASE) {
		Attack();
		return;
	}
	if (data.button == GLFW_MOUSE_BUTTON_RIGHT && data.action == GLFW_RELEASE) {
		Unattack();
	}
}

void Player::PokeAttackState::Update(double dt) {
	State::Update(dt);
	if (selectedPokemon->IsKO()) {
		player->context->gui->GetGameInfosField()->AddMessage("Selected Pokemon " + selectedPokemon->GetName() + " died.");
		player->EndTurn();
		return;
	}

	SharedContext* context = player->context;
	BattleGUI* gui = context->gui;
	/*if (gui->GetSelectedPokemonGUI()->GetStepClicked()) {
		Move();
		return;
	}*/
	{
		int moveId = gui->GetSelectedPokemonGUI()->GetMoveClicked();
		if (moveId != -1)
			SetMove(moveId);
	}

	std::vector<Pokemon*> attackedPokemon = context->board->GetPokemonCollision(cursor.GetIntRect());
	context->gui->GetSelectedPokemonGUI()->SetAimedPoke(attackedPokemon, move);
	
	UpdateCursor();
}

void Player::PokeAttackState::Render(){
	State::Render();
	for (Box*& box : rangeArea.GetBoxes()) {
		glm::ivec2 pos = box->GetPos();
		rangeBox.SetPosition(glm::ivec3(
			pos.x * Constants::BOX_SIZE,
			0.0f,
			pos.y * Constants::BOX_SIZE
		));
		player->context->win->Draw(rangeBox);
	}
	RenderCursor();
}

void Player::PokeAttackState::Move() {
	if (player->pokemonState[selectedPokemon].nbStepLeft == 0) {
		player->context->gui->GetGameInfosField()->AddMessage("No step left");
		return;
	}
	player->context->gui->GetSelectedPokemonGUI()->SetSelectedMove(-1);
	player->SwitchState(new PokeMoveState(player, selectedPokemon));
}

void Player::PokeAttackState::Attack() {
	Board* board = player->context->board;
	std::vector<Pokemon*> attackedPokemon = board->GetPokemonCollision(cursor.GetIntRect());
	bool touch = false;
	for (Pokemon* poke : attackedPokemon) {
		if (rangeArea.IsIn(board->GetPokemonPosition(poke))) {
			player->context->gameSystem->Attack(selectedPokemon, poke, move);
			touch = true;
		}
	}
	if (!touch)
		return;
	player->pokemonState[selectedPokemon].nbMove++;
	player->pokemonState[selectedPokemon].lock = true;
	player->context->gui->GetSelectedPokemonGUI()->SetSelectedMove(-1);
	player->SwitchState(new PokeSelectedState(player, selectedPokemon));
}

void Player::PokeAttackState::Unattack() {
	player->context->gui->GetGameInfosField()->AddMessage("Unattack " + selectedPokemon->GetName());
	player->context->gui->GetSelectedPokemonGUI()->SetSelectedMove(-1);
	player->SwitchState(new PokeSelectedState(player, selectedPokemon));
}