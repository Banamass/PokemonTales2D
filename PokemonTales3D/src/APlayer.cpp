#include "APlayer.h"
#include "BattleState.h"

/*------------------------APlayer------------------------*/

APlayer::APlayer(SharedContext* l_context)
	: context(l_context), isPlaying(false) {

}
APlayer::~APlayer() {
	for (Pokemon* poke : pokemons) {
		delete poke;
	}
}

bool APlayer::AddPokemon(Pokemon* poke, glm::ivec2 initialPos) {
	for (auto& p : pokemons)
		if (p == poke)
			return false;
	if (!context->board->SetPokemonPos(poke, initialPos))
		return false;
	pokemons.push_back(poke);
	pokemonState.emplace(poke, PokemonState());
	poke->SetMovePool(0, context->gameData->GetMoveData(1));
	poke->SetMovePool(1, context->gameData->GetMoveData(2));
	return true;
}

void APlayer::Render() {
	for (Pokemon*& poke : pokemons) {
		poke->Render(context->win, context->board->GetPokemonPosition(poke));
	}
}

void APlayer::PlayTurn() {
	isPlaying = true;
	for (auto& itr : pokemonState) {
		itr.second.lock = false;
		itr.second.nbStepLeft = itr.first->GetMoveRange();
		itr.second.nbMove = 0;
	}
}

void APlayer::EndTurn() {
	isPlaying = false;
}

void APlayer::PokemonKO(Pokemon* poke) {
	bool b = false;
	for (auto itr = pokemons.begin(); itr != pokemons.end(); itr++) {
		if (*itr == poke) {
			pokemons.erase(itr);
			b = true;
			break;
		}
	}
	auto itr = pokemonState.find(poke);
	if (itr != pokemonState.end())
		pokemonState.erase(itr);

	if (b) {
		context->gui->GetGameInfosField()->AddMessage(poke->GetName() + " is KO");
		//context->board->RemovePokemon(poke);
	}
}

bool APlayer::Playing() { return isPlaying; }
std::vector<Pokemon*>& APlayer::GetPokemons() { return pokemons; }