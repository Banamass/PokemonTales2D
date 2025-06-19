#include "Player.h"

/*-----------------------------Player-----------------------------*/

Player::Player(SharedContext* l_context) : APlayer(l_context), state(nullptr){
	pokemons.push_back(new Pokemon(20, sf::Color::Red, sf::Vector2i({2,2})));
	pokemons.push_back(new Pokemon(20, sf::Color::Green, sf::Vector2i({ 3,2 })));

	SetBindings();

	SwitchState(new DefaultState(this));
}
Player::~Player() {

}

void Player::Setup() {
	context->board->SetPokemonPos(pokemons[0], { 5,5 });
	context->board->SetPokemonPos(pokemons[1], { 10,10 });
}

void Player::Update() {

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
}

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
	std::cout << "Set default state callbacks " << std::endl;
	player->context->eventManager->AddCallback("MouseLClick", &Player::DefaultState::Select, this);
}
void Player::DefaultState::UnsetCallbacks() {
	std::cout << "UnSet default state callbacks " << std::endl;
	player->context->eventManager->RemoveCallback("MouseLClick");
}

void Player::DefaultState::Select() {
	std::cout << "Select" << std::endl;
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
	std::cout << "Set poke select state callbacks " << std::endl;
	player->context->eventManager->AddCallback("MouseMove", &Player::PokeSelectedState::UpdateCursor, this);
	player->context->eventManager->AddCallback("MouseRClick", &Player::PokeSelectedState::Unselect, this);
	player->context->eventManager->AddCallback("AClick", &Player::PokeSelectedState::Move, this);

	UpdateCursor();
}
void Player::PokeSelectedState::UnsetCallbacks() {
	std::cout << "UnSet poke select state callbacks " << std::endl;
	player->context->eventManager->RemoveCallback("MouseMove");
	player->context->eventManager->RemoveCallback("MouseRClick");
	player->context->eventManager->RemoveCallback("AClick");
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

/*-----------------------------PokeMoveState-----------------------------*/

Player::PokeMoveState::PokeMoveState(Player* l_player, Pokemon* l_selectedPokemon) 
	: State(l_player), selectedPokemon(l_selectedPokemon), moveArea(5, sf::Color(255,255, 255, 200)) {
	type = StateType::PokeMove;
	pokeFrame = selectedPokemon->GetFrame();
}

void Player::PokeMoveState::SetCallbacks() {
	std::cout << "Set poke move state callbacks " << std::endl;
	player->context->eventManager->AddCallback("MouseRClick", &Player::PokeMoveState::Unmove, this);
	player->context->eventManager->AddCallback("MouseLClick", &Player::PokeMoveState::Move, this);

	moveArea.Update(player->context->board, player->context->board->GetPokemonPosition(selectedPokemon));
}
void Player::PokeMoveState::UnsetCallbacks() {
	std::cout << "Set poke move state callbacks " << std::endl;
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
	if (!moveArea.IsIn(mousePos))
		return;
	bool hasMoved = player->context->board->SetPokemonPos(selectedPokemon, mousePos);
	if (!hasMoved)
		return;
	moveArea.Update(player->context->board, player->context->board->GetPokemonPosition(selectedPokemon));
	player->SwitchState(new PokeSelectedState(player, selectedPokemon));
}