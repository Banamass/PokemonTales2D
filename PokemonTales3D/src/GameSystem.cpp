#include "GameSystem.h"
#include "Camera.h"
#include "BattleState.h"
#include "OptionsState.h"

GameSystem::GameSystem(SharedContext* l_context)
	: context(l_context), boardSize(20, 20), board(boardSize, l_context),
	player(context), opponent(context), hoverPoke(nullptr){
	l_context->gameSystem = this;
	playingPlayer = nullptr;
}
GameSystem::~GameSystem(){
	
}

void GameSystem::StartBattle() {
	DataManager* dataManager = context->gameData;

	OptionsState* optionsState = (OptionsState*)context->stateManager->GetState(StateType::Options);
	OptionsData optData = optionsState->GetOptionsData();
	Pokemon* addPoke = nullptr;
	for (auto& poke : optData.pokeNamePlayer1) {
		if (!poke)
			continue;
		std::cout << "Name : " << poke->name << ", id : " << poke->id << " for player" <<  std::endl;
		glm::vec3 color((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
		addPoke = new Pokemon(poke, &player, context->modelManager
			, context->shaderManager, color);
		glm::ivec2 pos(rand() % (board.GetSize().x / 2), rand() % (board.GetSize().y / 2));
		while (!player.AddPokemon(addPoke, pos)) {
			pos = glm::ivec2(rand() % (board.GetSize().x / 2), rand() % (board.GetSize().y / 2));
		}
	}
	for (auto& poke : optData.pokeNamePlayer2) {
		if (!poke)
			continue;
		std::cout << "Name : " << poke->name << ", id : " << poke->id  << " for opponent" << std::endl;
		glm::vec3 color((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
		addPoke = new Pokemon(poke, &opponent, context->modelManager
			, context->shaderManager, color);
		glm::ivec2 pos(rand() % (board.GetSize().x / 2), rand() % (board.GetSize().y / 2));
		while (!opponent.AddPokemon(addPoke, pos)) {
			pos = glm::ivec2(rand() % (board.GetSize().x / 2), rand() % (board.GetSize().y / 2));
		}
	}

	player.SetPlayerColor(optData.player1Color);
	opponent.SetPlayerColor(optData.player2Color);

	playingPlayer = &player;
	player.PlayTurn();
}

void GameSystem::UpdatePlayingPlayer() {
	if (!playingPlayer->Playing()) {
		if (playingPlayer == &player) {
			context->gui->GetGameInfosField()->AddMessage("Opponent is playing his turn");
			playingPlayer = &opponent;
		}
		else {
			context->gui->GetGameInfosField()->AddMessage("Player is playing his turn");
			playingPlayer = &player;
		}
		playingPlayer->PlayTurn();
	}
}
void GameSystem::UpdateHoverPokemon() {
	glm::vec3 mouseDir = context->camera->GetMouseDirection();
	glm::vec3 camPos = context->camera->GetPosition();

	bool hover = false;
	float d = 100000000000.0f;
	Pokemon* p = nullptr;
	for (auto*& poke : player.GetPokemons()) {
		float temp = d;
		if (poke->TestRayIntersection(camPos, mouseDir, d) && d < temp) {
			if (poke != hoverPoke) {
				p = poke;
			}
			hover = true;
		}
	}
	for (auto*& poke : opponent.GetPokemons()) {
		float temp = d;
		if (poke->TestRayIntersection(camPos, mouseDir, d) && d < temp) {
			if (poke != hoverPoke) {
				p = poke;
			}
			hover = true;
		}
	}
	if (hover && p != nullptr) {
		HoverPokemon(p);
	}
	if (!hover) {
		UnHoverPokemon();
	}
}

void GameSystem::HoverPokemon(Pokemon* poke) {
	hoverPoke = poke;
	context->gui->SetHoverPokemon(poke);
}
void GameSystem::UnHoverPokemon() {
	if (hoverPoke == nullptr)
		return;
	context->gui->UnsetHoverPokemon();
	hoverPoke = nullptr;
}

void GameSystem::Update(double dt){
	UpdatePlayingPlayer();
	UpdateHoverPokemon();

	board.Update(dt);
	playingPlayer->Update(dt);
}

void GameSystem::Render(){
	board.Draw();
	player.Render();
	opponent.Render();
}

void GameSystem::Attack(Pokemon* attacker, Pokemon* attacked, PokemonMove* move) {
	float damage = ComputeDamages(attacker, attacked, move);
	std::stringstream mess;
	mess << attacker->GetName() << " inflicts " << damage << " damages to " << attacked->GetName();
	context->gui->GetGameInfosField()->AddMessage(mess.str());
	attacked->TakeDamages(damage);
}

float GameSystem::ComputeDamages(Pokemon* attacker, Pokemon* attacked, PokemonMove* move) {
	return move->pp;
}