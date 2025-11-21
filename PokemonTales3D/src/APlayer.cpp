#include "APlayer.h"
#include "BattleState.h"

/*------------------------APlayer------------------------*/

APlayer::APlayer(SharedContext* l_context)
	: context(l_context), isPlaying(false)
{
	SetPlayerColor(glm::vec3(1.0f));

	pokemonsMark.SetModel(l_context->modelManager->RequireGetResource("Mark"));
	pokemonsMark.SetShader(l_context->shaderManager->GetShader("ModelShader"));
	pokemonsMark.Scale(0.4f);
}
APlayer::~APlayer() {
	context->modelManager->ReleaseResource("Mark");
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
	poke->SetMovePool(2, context->gameData->GetMoveData(3));
	return true;
}

void APlayer::Render() {
	for (Pokemon*& poke : pokemons) {
		glm::ivec2 pos = context->board->GetPokemonPosition(poke);
		poke->Render(context->win, pos);
		pokemonsMark.SetPosition(
			glm::vec3(pos.x * Constants::BOX_SIZE, 2.0f, pos.y * Constants::BOX_SIZE));
		context->win->Draw(pokemonsMark);
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

void APlayer::SetPlayerColor(glm::vec3 color) {
	playerColor = color;
	Drawable::Material mat;
	mat.SetLightningColor(playerColor, 0.5f);
	pokemonsMark.SetMaterial(mat);
}
bool APlayer::Playing() { return isPlaying; }
std::vector<Pokemon*>& APlayer::GetPokemons() { return pokemons; }