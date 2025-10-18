#pragma once

#include "StateManager.h"
#include "EventManager.h"
#include "GUI.h"
#include <vector>

//Data from options state that can be retrieved from another state
struct OptionsData {
	std::vector<const PokemonData*> pokeNamePlayer1;
	std::vector<const PokemonData*> pokeNamePlayer2;
};

//GUI for setting all options relative to one player
class PlayerOptionsGUI : public Panel {
public:
	PlayerOptionsGUI(SharedContext* l_context, std::string l_playerTag);
	virtual ~PlayerOptionsGUI();

	void Update(Window* win);
	void Scroll(int xoffset, int yoffset);

	//Set if the options are actived and can be changed
	void SetActivated(bool b);

	//Return the name of selected Pokemons
	std::vector<std::string> GetSelectedPokemon();
	//Reset all the options to the default configuration
	void Reset();

private:
	std::string playerTag;

	std::string defaultSelection;
	std::string noneField;

	std::vector<SelectBox*> pokeSelection;
	Text* playerTagText;

	bool activated;
};

//This class implement State for the options state 
class OptionsState : public State {
public:
	OptionsState(SharedContext* l_context);
	virtual ~OptionsState();

	virtual void Update(double dt);
	virtual void Render();

	virtual void Activate();

	//Callbacks

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