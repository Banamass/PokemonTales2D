#pragma once

#include "StateManager.h"
#include "EventManager.h"
#include "GUI.h"
#include <vector>

struct OptionsData {
	std::vector<const PokemonData*> pokeNamePlayer1;
	std::vector<const PokemonData*> pokeNamePlayer2;
};

class PlayerOptionsGUI : public Panel {
public:
	PlayerOptionsGUI(SharedContext* l_context);
	virtual ~PlayerOptionsGUI();

	void Update(Window* win);
	void Scroll(int xoffset, int yoffset);

	void SetActivated(bool b);

	//Return the name of selected Pokemons
	std::vector<std::string> GetSelectedPokemon();
	void Reset();

private:
	std::string defaultSelection;
	std::string noneField;

	std::vector<SelectBox*> pokeSelection;

	bool activated;
};

class OptionsState : public State {
public:
	OptionsState(SharedContext* l_context);
	virtual ~OptionsState();

	virtual void Update(double dt);
	virtual void Render();

	virtual void Activate();

	void KeyCallback(CallbackData data);
	void MouseButtonCallback(CallbackData data);
	void ScrollCallback(CallbackData data);

	const OptionsData& GetOptionsData();

private:
	void SetUpGeneralButtons();

	void Quit();
	void Restart();
	void SetBattleState(bool isInProgress);

	OptionsData data;

	PlayerOptionsGUI player1Options;
	PlayerOptionsGUI player2Options;

	Panel generalButtons;
	Button* restartButton;
	Button* okButton;
	Button* quitButton;

	Text* battleState;
};