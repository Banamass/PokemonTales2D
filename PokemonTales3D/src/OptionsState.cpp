#include "OptionsState.h"
#include "Game.h"

OptionsState::OptionsState(SharedContext* l_context)
	: State(l_context)
{
	type = StateType::Options;

	context->eventManager->AddCallback("Options", EventType::Key, &OptionsState::KeyCallback, this, StateType::Options);

	glm::vec2 selectSize(200.0f, 50.0f);
	glm::vec2 select1Pos(10.0f, 100.0f);
	glm::vec2 select2Pos(10.0f, 400.0f);
	float space = 40.0f;

	std::vector<std::string> pokemonsName;
	const std::vector<PokemonData>& data = context->gameData->GetAllPokemonData();
	for (auto& d : data) {
		pokemonsName.push_back(d.name);
	}

	for (int i = 0; i < Constants::NB_POKEMON_BY_PLAYER; i++) {
		pokeSelectPlayer1.push_back(new SelectBox(
			context->fontManager->GetResource("Arial"),
			context->shaderManager, pokemonsName, selectSize
		));
		pokeSelectPlayer1[i]->SetPos(glm::vec2(
			select1Pos.x + (selectSize.x + space) * i, select1Pos.y
		));

		pokeSelectPlayer2.push_back(new SelectBox(
			context->fontManager->GetResource("Arial"),
			context->shaderManager, pokemonsName, selectSize
		));
		pokeSelectPlayer2[i]->SetPos(glm::vec2(
			select2Pos.x + (selectSize.x + space) * i, select2Pos.y
		));
	}
}
OptionsState::~OptionsState(){
	context->eventManager->RemoveCallbacks("Options");

	for (auto& s : pokeSelectPlayer1)
		delete s;
	for (auto& s : pokeSelectPlayer2)
		delete s;
}

void OptionsState::Update(double dt){
	for (auto& s : pokeSelectPlayer1)
		s->Update(context->win);
	for (auto& s : pokeSelectPlayer2)
		s->Update(context->win);
}
void OptionsState::Render(){
	glDisable(GL_DEPTH_TEST);

	for (auto& s : pokeSelectPlayer1)
		context->win->DrawStatic(s);
	for (auto& s : pokeSelectPlayer2)
		context->win->DrawStatic(s);

	glEnable(GL_DEPTH_TEST);
}

void OptionsState::KeyCallback(CallbackData data){
	Key_Data kdata = std::get<Key_Data>(data.data);
	if (kdata.key == GLFW_KEY_ESCAPE && kdata.action == GLFW_RELEASE)
		context->game->SwitchState(StateType::Menu);
}
void OptionsState::MouseButtonCallback(CallbackData data){}