#pragma once

#include "GameSystem.h"
#include "StateManager.h"
#include "Camera.h"
#include "Light.h"

class PokemonGUI : public Panel {
public:
	PokemonGUI(Pokemon* l_poke, Font* l_font, SharedContext* l_context);
	virtual ~PokemonGUI();

	void Update(double dt);
	virtual void Draw(glm::mat4& cameraMatrix);

	void SetPokemon(Pokemon* poke);
	Pokemon* GetPokemon();
	//With i outside [0,3], unselect all move
	void SetSelectedMove(int i);
	void SetAimedPoke(std::vector<Pokemon*>& aimedPoke, PokemonMove* move);
	void SetNbStepsLeft(int l_nbStep);
	//Return the id of the clicked move, -1 if no move is clicked
	int GetMoveClicked();
	//Get if the step button has been clicked
	bool GetStepClicked();

private:
	void Reset();

	Pokemon* poke;
	SharedContext* context;

	PokemonStatsBar* statsBar;
	Panel* movesBar;
	PokemonMoveBar* moveBars[4];
	int selectedMove; //-1 if no move is selected
	static const int MAX_AIMED_POKE = 10;
	PokemonStatsBar* aimedPokeStatsBar[MAX_AIMED_POKE];
	Panel* nbStepBar;
	Text* nbStepText;
	Button* stepButton;
	int nbStepLeft;
};

class BattleGUI {
public:
	BattleGUI(SharedContext* l_context);
	virtual ~BattleGUI();

	void Update(double dt);
	void Render();

	TextField* GetGameInfosField();
	PokemonGUI* GetSelectedPokemonGUI();

	void Reset();

	void SetHoverPokemon(Pokemon* poke);
	void UnsetHoverPokemon();
	void SetSelectedPokemon(Pokemon* poke);
	void UnsetSelectedPokemon();

private:
	SharedContext* context;
	Font* font;

	Panel gameName;

	PokemonStatsBar hoverPokeBar;

	PokemonGUI selectedPokeGUI;

	RectangleShape cursor;
	TextField gameInfos;
};

struct BattleData {
	bool battleInProgress;
};

class BattleState : public State {
public:
	BattleState(SharedContext* l_context);
	virtual ~BattleState();

	virtual void Update(double dt);
	virtual void Render();

	virtual void Activate();

	void KeyCallback(CallbackData data);

	void Restart();
	BattleData GetBattleData();

private:

	Camera camera;

	Light light;

	Cubemap* skybox;
	Drawable floor;

	GameSystem* gameSystem;
	BattleGUI gui;

	bool inProgress;
};