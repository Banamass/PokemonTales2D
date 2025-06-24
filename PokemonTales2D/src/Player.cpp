#include "Player.h"
#include "GameSystem.h"

/*-----------------------------Player-----------------------------*/

Player::Player(SharedContext* l_context) : APlayer(l_context), state(nullptr){
	SetBindings();
	SetGlobalCallbacks();
}
Player::~Player() {
	UnsetGlobalCallbacks();
}

void Player::Setup() {
	pokemons.push_back(new Pokemon("Carapuce", 20, sf::Color::Red, sf::Vector2i({ 2,2 }), this));
	pokemons.push_back(new Pokemon("Salamèche", 20, sf::Color::Green, sf::Vector2i({ 3,2 }), this));
	pokemons[0]->SetMoveRange(2);
	pokemons[1]->SetMoveRange(3);

	context->board->SetPokemonPos(pokemons[0], { 5,5 });
	context->board->SetPokemonPos(pokemons[1], { 10,10 });
}

void Player::PlayTurn() {
	APlayer::PlayTurn();
	SwitchState(new DefaultState(this));
}

void Player::Render() {
	state->Render();
}

void Player::SetBindings() {
	Binding* b = new Binding("MouseLClick");
	b->BindEvent(new MouseEvent(MouseEvent::LClick));
	context->eventManager->AddBinding(b);

	b = new Binding("MouseMove");
	b->BindEvent(new MouseEvent(MouseEvent::Moved));
	context->eventManager->AddBinding(b);

	b = new Binding("MouseRClick");
	b->BindEvent(new MouseEvent(MouseEvent::RClick));
	context->eventManager->AddBinding(b);

	b = new Binding("AClick");
	b->BindEvent(new KeyboardEvent(KeyboardEvent::Pressed, sf::Keyboard::Key::A));
	context->eventManager->AddBinding(b);

	b = new Binding("Num1Click");
	b->BindEvent(new KeyboardEvent(KeyboardEvent::Pressed, sf::Keyboard::Key::Num1));
	context->eventManager->AddBinding(b);

	b = new Binding("QClick");
	b->BindEvent(new KeyboardEvent(KeyboardEvent::Pressed, sf::Keyboard::Key::Q));
	context->eventManager->AddBinding(b);
}

void Player::SetGlobalCallbacks() {
	context->eventManager->AddCallback("QClick", &Player::EndTurn, this);
}

void Player::UnsetGlobalCallbacks() {
	context->eventManager->RemoveCallback("QClick");
}

void Player::EndTurn() { isPlaying = false; }

void Player::SwitchState(State* newState) {
	if (newState == nullptr)
		return;
	StateType type = newState->GetType();
	if (type == StateType::None)
		return;
	if (state != nullptr) {
		state->UnsetCallbacks();
		delete state;
	}
	state = newState;
	state->SetCallbacks();
}

/*-----------------------------DefaultState-----------------------------*/

Player::DefaultState::DefaultState(Player* l_player) : State(l_player) {
	type = StateType::Default;
}

void Player::DefaultState::SetCallbacks() {
	player->context->eventManager->AddCallback("MouseLClick", &Player::DefaultState::Select, this);
}
void Player::DefaultState::UnsetCallbacks() {
	player->context->eventManager->RemoveCallback("MouseLClick");
}

void Player::DefaultState::Select() {
	Pokemon* poke = player->context->board->GetPokemonFromPos(player->context->board->GetMousePosition());
	if (poke == nullptr)
		return;
	player->SwitchState(new PokeSelectedState(player, poke));
}

/*-----------------------------PokeSelectedState-----------------------------*/

Player::PokeSelectedState::PokeSelectedState(Player* l_player, Pokemon* l_selectedPokemon) 
	: State(l_player), selectedPokemon(l_selectedPokemon), cursor(sf::Vector2i({1, 1}), sf::Color::Blue) {
	type = StateType::PokeSelected;
}

void Player::PokeSelectedState::SetCallbacks() {
	player->context->eventManager->AddCallback("MouseMove", &Player::PokeSelectedState::UpdateCursor, this);
	player->context->eventManager->AddCallback("MouseRClick", &Player::PokeSelectedState::Unselect, this);
	player->context->eventManager->AddCallback("AClick", &Player::PokeSelectedState::Move, this);
	player->context->eventManager->AddCallback("Num1Click", &Player::PokeSelectedState::Attack, this);

	UpdateCursor();
}
void Player::PokeSelectedState::UnsetCallbacks() {
	player->context->eventManager->RemoveCallback("MouseMove");
	player->context->eventManager->RemoveCallback("MouseRClick");
	player->context->eventManager->RemoveCallback("AClick");
	player->context->eventManager->RemoveCallback("Num1Click");
}

void Player::PokeSelectedState::UpdateCursor() {
	cursor.Update(player->context->board, player->context->board->GetMousePosition());
}

void Player::PokeSelectedState::Unselect() {
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
	: State(l_player), selectedPokemon(l_selectedPokemon), moveArea(l_selectedPokemon, sf::Color(255,255, 255, 200)) {
	type = StateType::PokeMove;
	pokeFrame = selectedPokemon->GetFrame();
}

void Player::PokeMoveState::SetCallbacks() {
	player->context->eventManager->AddCallback("MouseRClick", &Player::PokeMoveState::Unmove, this);
	player->context->eventManager->AddCallback("MouseLClick", &Player::PokeMoveState::Move, this);

	moveArea.Update(player->context->board, player->context->board->GetPokemonPosition(selectedPokemon));
}
void Player::PokeMoveState::UnsetCallbacks() {
	player->context->eventManager->RemoveCallback("MouseRClick");
	player->context->eventManager->RemoveCallback("MouseLClick");
}

void Player::PokeMoveState::Render() {
	player->context->board->Draw(pokeFrame, player->context->board->GetMousePosition());
}

void Player::PokeMoveState::Unmove() {
	player->SwitchState(new PokeSelectedState(player, selectedPokemon));
}

void Player::PokeMoveState::Move() {
	sf::Vector2i mousePos = player->context->board->GetMousePosition();
	if (moveArea.Distance(player->context->board, mousePos) < 0)
		return;
	bool hasMoved = player->context->board->SetPokemonPos(selectedPokemon, mousePos);
	if (!hasMoved)
		return;
	moveArea.Update(player->context->board, player->context->board->GetPokemonPosition(selectedPokemon));
	player->SwitchState(new PokeSelectedState(player, selectedPokemon));
}

/*-----------------------------PokeAttackState-----------------------------*/
Player::PokeAttackState::PokeAttackState(Player* l_player, Pokemon* l_selectedPokemon)
	: State(l_player), selectedPokemon(l_selectedPokemon), cursor({ 3,3}, sf::Color::Red) {
	type = StateType::PokeAttack;
}

void Player::PokeAttackState::SetCallbacks() {
	player->context->eventManager->AddCallback("MouseLClick", &Player::PokeAttackState::Attack, this);
	player->context->eventManager->AddCallback("MouseMove", &Player::PokeAttackState::UpdateCursor, this);

	cursor.Update(player->context->board, player->context->board->GetPokemonPosition(selectedPokemon));
}
void Player::PokeAttackState::UnsetCallbacks() {
	player->context->eventManager->RemoveCallback("MouseLClick");
	player->context->eventManager->RemoveCallback("MouseMove");
}

void Player::PokeAttackState::Attack() {
	Board* board = player->context->board;
	std::vector<Pokemon*> attackedPokemon = board->GetPokemonCollision(cursor.GetIntRect());
	for (Pokemon* poke : attackedPokemon) {
		player->context->gameSystem->Attack(selectedPokemon, poke);
	}
}

void Player::PokeAttackState::UpdateCursor() {
	cursor.Update(player->context->board, player->context->board->GetMousePosition());
}