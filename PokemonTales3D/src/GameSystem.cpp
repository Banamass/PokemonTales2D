#include "GameSystem.h"
#include "Camera.h"
#include "BattleState.h"

GameSystem::GameSystem(SharedContext* l_context)
	: context(l_context), board(glm::vec2(20, 20), l_context),
	player(context), opponent(context), hoverPoke(nullptr){
	l_context->gameSystem = this;
	DataManager* dataManager = context->gameData;

	playingPokemons.push_back(new Pokemon(dataManager->GetPokemonData(1)
		, &player, context->modelManager, context->shaderManager, glm::vec3(0.0f, 0.0f, 1.0f)));
	playingPokemons.push_back(new Pokemon(dataManager->GetPokemonData(1)
		, &player, context->modelManager, context->shaderManager, glm::vec3(1.0f, 0.0f, 0.0f)));
	player.AddPokemon(playingPokemons[0], { 0,0 });
	player.AddPokemon(playingPokemons[1], { 2,2 });

	playingPokemons.push_back(new Pokemon(dataManager->GetPokemonData(1)
		, &opponent, context->modelManager, context->shaderManager, glm::vec3(0.0f, 1.0f, 0.0f)));
	opponent.AddPokemon(playingPokemons[2], { 5,3 });

	playingPlayer = &player;
	player.PlayTurn();
}
GameSystem::~GameSystem(){
	for (auto& poke : playingPokemons) {
		delete poke;
	}
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
	for (Pokemon* poke : player.GetPokemons()) {
		if (poke->IsKO()) {
			context->gui->GetGameInfosField()->AddMessage(poke->GetName() + " is KO");
			player.PokemonKO(poke);
			board.RemovePokemon(poke);
		}
	}
	for (Pokemon* poke : opponent.GetPokemons()) {
		if (poke->IsKO()) {
			context->gui->GetGameInfosField()->AddMessage(poke->GetName() + " is KO");
			opponent.PokemonKO(poke);
			board.RemovePokemon(poke);
		}
	}
}
void GameSystem::UpdateHoverPokemon() {
	glm::vec3 mouseDir = context->camera->GetMouseDirection();
	glm::vec3 camPos = context->camera->GetPosition();

	bool hover = false;
	float d = 100000000000.0f;
	for (auto*& poke : playingPokemons) {
		float temp = d;
		if (poke->TestRayIntersection(camPos, mouseDir, d)) {
			if (d < temp){
				if (poke != hoverPoke) {
					HoverPokemon(poke);
				}
				hover = true;
			}
		}
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