#include "Player.h"
#include "GameSystem.h"

/*-----------------------------Player-----------------------------*/

Player::Player(SharedContext* l_context) 
	: APlayer(l_context), state(nullptr), boxModel("Resources\\Box\\box.obj")
{
	Setup();
}
Player::~Player() {
	
}

void Player::Setup() {
	context->eventManager->AddCallback(EventType::Key, &Player::KeyCallback, this);
	context->eventManager->AddCallback(EventType::MouseButton, &Player::MouseButtonCallback, this);
}

void Player::KeyCallback(CallbackData data) {
	if (isPlaying) {
		Key_Data kdata = std::get<Key_Data>(data.data);
		if (kdata.key == GLFW_KEY_1 && kdata.action == GLFW_PRESS) {
			context->gui->GetGameInfosField()->AddMessage("Fin de tour");
			EndTurn();
			return;
		}
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
	if(state != nullptr)
		state->Update(dt);
}

void Player::Render() {
	APlayer::Render();
	if(state != nullptr)
		state->Render();
}

void Player::EndTurn() { 
	isPlaying = false; 
	context->gui->SetSelectedPokemon(nullptr);
}

void Player::SwitchState(State* newState) {
	if (newState == nullptr)
		return;
	StateType type = newState->GetType();
	if (type == StateType::None)
		return;
	if (state != nullptr) {
		delete state;
	}
	state = newState;
}

/*-----------------------------State-----------------------------*/

void Player::State::UpdateCursor() {
	Board* board = player->context->board;
	cursor.Update(board, board->GetMousePos());
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

/*-----------------------------DefaultState-----------------------------*/

Player::DefaultState::DefaultState(Player* l_player) 
	: State(l_player)
		{
	type = StateType::Default;

	Drawable::Material mat;
	mat.ambient = 0.3f * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	mat.diffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	mat.specular = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	mat.shininess = 32.0f;
	cursorDrawable.SetMaterial(mat);
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
	UpdateCursor();
}

void Player::DefaultState::Render()
{
	RenderCursor();
}

void Player::DefaultState::Select() {
	Pokemon* poke = player->context->board->GetPokemonFromPos(cursor.GetIntRect().pos);
	if (poke == nullptr || poke->GetTrainer() != player)
		return;
	player->context->gui->GetGameInfosField()->AddMessage("Select " + poke->GetName());
	player->context->gui->SetSelectedPokemon(poke);
	player->SwitchState(new PokeSelectedState(player, poke));
}

/*-----------------------------PokeSelectedState-----------------------------*/

Player::PokeSelectedState::PokeSelectedState(Player* l_player, Pokemon* l_selectedPokemon)
	: State(l_player), selectedPokemon(l_selectedPokemon) {
	type = StateType::PokeSelected;
}

void Player::PokeSelectedState::KeyCallback(Key_Data& data) {
	if (data.key == GLFW_KEY_2 && data.action == GLFW_RELEASE)
		Unselect();
	else if (data.key == GLFW_KEY_3 && data.action == GLFW_RELEASE)
		Move();
	else if (data.key == GLFW_KEY_4 && data.action == GLFW_RELEASE)
		Attack();
}

void Player::PokeSelectedState::Unselect() {
	player->context->gui->GetGameInfosField()->AddMessage("Unselect " + selectedPokemon->GetName());
	player->context->gui->SetSelectedPokemon(nullptr);
	player->SwitchState(new DefaultState(player));
}

void Player::PokeSelectedState::Move() {
	player->SwitchState(new PokeMoveState(player, selectedPokemon));
}

void Player::PokeSelectedState::Attack() {
	player->SwitchState(new PokeAttackState(player, selectedPokemon));
}

/*-----------------------------PokeMoveState-----------------------------*/

Player::PokeMoveState::PokeMoveState(Player* l_player, Pokemon* l_selectedPokemon)
	: State(l_player), selectedPokemon(l_selectedPokemon), moveArea(l_selectedPokemon),
	moveBox(&player->boxModel, player->context->shaderManager->GetShader("ModelShader"))
{
	type = StateType::PokeMove;
	//pokeFrame = selectedPokemon->GetFrame();

	Drawable::Material mat;
	glm::vec4 color(0.0f, 1.0f, 0.0f, 1.0f);
	mat.ambient = 0.3f * color;
	mat.diffuse = color;
	mat.specular = glm::vec4(1.0);
	mat.shininess = 32.0f;
	cursorDrawable.SetMaterial(mat);

	color = glm::vec4(0.5f, 0.0f, 1.0f, 1.0f);
	mat.ambient = 0.3f * color;
	mat.diffuse = color;
	moveBox.SetMaterial(mat);
	float scale = 0.95f;
	moveBox.Scale(glm::vec3(scale, scale, scale));

	moveArea.Update(player->context->board, player->context->board->GetPokemonPosition(selectedPokemon));
}

void Player::PokeMoveState::KeyCallback(Key_Data& data) {
	if (data.key == GLFW_KEY_2 && data.action == GLFW_RELEASE)
		Unmove();
}
void Player::PokeMoveState::MouseButtonCallback(MouseButton_Data& data) {
	if (data.button == GLFW_MOUSE_BUTTON_LEFT && data.action == GLFW_RELEASE)
		Move();
}

void Player::PokeMoveState::Render() {
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
	UpdateCursor();
}

void Player::PokeMoveState::Unmove() {
	player->context->gui->GetGameInfosField()->AddMessage("Unmove " + selectedPokemon->GetName());
	player->SwitchState(new PokeSelectedState(player, selectedPokemon));
}

void Player::PokeMoveState::Move() {
	
	glm::ivec2 movePos = cursor.GetIntRect().pos;
	if (moveArea.Distance(player->context->board, movePos) < 0)
		return;
	bool hasMoved = player->context->board->SetPokemonPos(selectedPokemon, movePos);
	if (!hasMoved)
		return;
	moveArea.Update(player->context->board, player->context->board->GetPokemonPosition(selectedPokemon));
	player->context->gui->GetGameInfosField()->AddMessage("Move " + selectedPokemon->GetName());
	player->SwitchState(new PokeSelectedState(player, selectedPokemon));
}

/*-----------------------------PokeAttackState-----------------------------*/
Player::PokeAttackState::PokeAttackState(Player* l_player, Pokemon* l_selectedPokemon)
	: State(l_player), selectedPokemon(l_selectedPokemon) {
	type = StateType::PokeAttack;

	Drawable::Material mat;
	glm::vec4 color(1.0f, 1.0f, 0.0f, 1.0f);
	mat.ambient = 0.3f * color;
	mat.diffuse = color;
	mat.specular = glm::vec4(1.0);
	mat.shininess = 32.0f;
	cursorDrawable.SetMaterial(mat);
	cursor.SetSize(glm::ivec2(2, 1));
}

void Player::PokeAttackState::KeyCallback(Key_Data& data) {
	if (data.key == GLFW_KEY_2 && data.action == GLFW_RELEASE) {
		Unattack();
	}
}

void Player::PokeAttackState::MouseButtonCallback(MouseButton_Data& data) {
	if (data.button == GLFW_MOUSE_BUTTON_LEFT && data.action == GLFW_RELEASE) {
		Attack();
		return;
	}
}

void Player::PokeAttackState::Update(double dt) {
	UpdateCursor();
}

void Player::PokeAttackState::Render()
{
	RenderCursor();
}

void Player::PokeAttackState::Attack() {
	Board* board = player->context->board;
	std::vector<Pokemon*> attackedPokemon = board->GetPokemonCollision(cursor.GetIntRect());
	for (Pokemon* poke : attackedPokemon) {
		player->context->gameSystem->Attack(selectedPokemon, poke);
	}
}

void Player::PokeAttackState::Unattack() {
	player->context->gui->GetGameInfosField()->AddMessage("Unattack " + selectedPokemon->GetName());
	player->SwitchState(new PokeSelectedState(player, selectedPokemon));
}