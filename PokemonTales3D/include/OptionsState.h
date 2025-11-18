#pragma once

#include "StateManager.h"
#include "EventManager.h"
#include "GUI.h"
#include <vector>

//Data from options state that can be retrieved from another state
struct OptionsData {
	std::vector<const PokemonData*> pokeNamePlayer1;
	std::vector<const PokemonData*> pokeNamePlayer2;
	glm::vec4 player1Color;
	glm::vec4 player2Color;
};

//GUI for setting all options relative to one player
class PlayerOptionsGUI : public Panel, public Notifier {
public:
	PlayerOptionsGUI(SharedContext* l_context, std::string l_playerTag);
	virtual ~PlayerOptionsGUI();

	void Update(Window* win);
	void Scroll(double xoffset, double yoffset);

	//Set if the options are actived and can be changed
	void SetActivated(bool b);

	//Add a new color to the player color selection
	void AddPlayerColor(glm::vec4 color);
	//Set the player color
	void SetSelectedPlayerColor(glm::vec4 color);

	//Set the other player option gui
	void SetOtherPlayerOption(PlayerOptionsGUI* l_otherPlayerOpt);

	//Return the name of selected Pokemons
	std::vector<std::string> GetSelectedPokemon();
	//Return the selected color for the player
	glm::vec4 GetPlayerColor();
	//Reset all the options to the default configuration
	void Reset();

private:
	//Notify functions
	void ColorSelectionNotify();
	void ColorOtherPlayerNotify();

	PlayerOptionsGUI* otherPlayerOpt;

	std::string playerTag;

	std::string defaultSelection;
	std::string noneField;

	std::vector<SelectBox*> pokeSelection;
	Text* playerTagText;
	ColorSelection* playerColorSelection;
	Text* textSelectColor;

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