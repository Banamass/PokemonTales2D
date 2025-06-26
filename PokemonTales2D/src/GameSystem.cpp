#include "GameSystem.h"

GameSystem::GameSystem(SharedContext* l_context)
	: board(l_context), context(l_context), player(l_context), opponent(l_context)
	, gameInfoField({400, 110}, 15, {10,10}) {
	context->board = &board;
	player.Setup();
	opponent.Setup();
	playingPlayer = &player;
	playingPlayer->PlayTurn();
}
GameSystem::~GameSystem() {

}

void GameSystem::Update() {
	if (!playingPlayer->Playing()) {
		if (playingPlayer == &player) {
			gameInfoField.AddMessage("Opponent is playing his turn");
			playingPlayer = &opponent;
		}
		else {
			gameInfoField.AddMessage("Player is playing his turn");
			playingPlayer = &player;
		}
		playingPlayer->PlayTurn();
	}
	for (Pokemon* poke : player.GetPokemons()) {
		if (poke->IsKO()) {
			gameInfoField.AddMessage(poke->GetName() + " is KO");
			player.PokemonKO(poke);
			board.RemovePokemon(poke);
		}
	}
	for (Pokemon* poke : opponent.GetPokemons()) {
		if (poke->IsKO()) {
			gameInfoField.AddMessage(poke->GetName() + " is KO");
			opponent.PokemonKO(poke);
			board.RemovePokemon(poke);
		}
	}
	for (Pokemon* poke : player.GetPokemons()) {

	}
	board.Update();
}
void GameSystem::Render() {
	board.Render();
	player.Render();
	context->window->StartDrawDefaultView();
	gameInfoField.Render(context->window);
	context->window->EndDrawDefaultView();
}

void GameSystem::Attack(Pokemon* attacker, Pokemon* attacked) {
	float damage = 10;
	std::stringstream mess;
	mess << attacker->GetName() << " inflicts " << damage << " damages to " << attacked->GetName();
	gameInfoField.AddMessage(mess.str());
	attacked->TakeDamages(10);
}