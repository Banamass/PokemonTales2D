#pragma once

#include "GUI.h"

class PokemonTypeFrame : public Panel {
public:
	PokemonTypeFrame(PokeType pokeType, Font* l_font, ShaderManager* l_shaderManager);
	virtual ~PokemonTypeFrame();

	void SetSize(glm::vec2 l_size);
	void SetCharacterSize(float charSize);
	void SetPokeType(PokeType l_pokeType);

private:
	glm::vec2 size;
	PokeType pokeType;

	Text* text;
	RectangleShape* frame;
};

class PokemonMoveBar : public Panel, public Clickable {
public:
	PokemonMoveBar(Font* l_font, ShaderManager* l_shaderMgr, glm::ivec2 l_pos);
	virtual ~PokemonMoveBar();

	//Draw the move bar
	virtual void Draw(glm::mat4& cameraMatrix);

	void Hover();
	void UnHover();

	virtual bool In(glm::vec2 mousePos);

	void SetPokemonMove(PokemonMove* l_move);
	glm::vec2 GetSize();
	void SetSelect(bool b);

private:
	PokemonMove* move;

	glm::vec2 typeFramePadding;
	float charSize;
	glm::vec2 size;
	Text* moveName;
	Text* ppText;
	Text* powerText;
	Text* rangeText;
	PokemonTypeFrame* typeFrame;
	RectangleShape* frame;

	bool isSelected;
	glm::vec4 selectedColor;
	glm::vec4 unselectedColor;
};

class PokemonStatsBar : public Panel {
public:
	PokemonStatsBar(Pokemon* l_poke, Font* l_font, ShaderManager* shaderMgr);
	virtual ~PokemonStatsBar();

	void Update(double dt);
	virtual void Draw(glm::mat4& cameraMatrix);

	void SetPokemon(Pokemon* poke);
	void SimulateDamages(float damages);

	glm::vec2 GetSize();
	Pokemon* GetPokemon();

protected:
	Font* font;

	Pokemon* poke;

	glm::vec2 size;
	Text* pokeName;
	Text* healthText;

	glm::vec2 healthBarSize;
	glm::vec2 healthBarPos;
	RectangleShape* healthBar;
	RectangleShape* healthBarFrame;
	RectangleShape* healthBarBack;
	RectangleShape* healthBarBackground;
	PokemonTypeFrame* type1Frame;
	PokemonTypeFrame* type2Frame;
	float simulatedDamages;
};

class PokemonStatsPanel : public WindowTabs {
public:
	PokemonStatsPanel(Pokemon* l_poke, Font* l_font, ShaderManager* l_shaderManager);
	virtual ~PokemonStatsPanel();

	void Draw(glm::mat4& cameraMat);
	void Update(Window* win);

	void SetPokemon(Pokemon* l_poke);
	void SetSize(glm::vec2 l_size);

private:
	Pokemon* poke;
	float padding;

	Panel* statsPanel;
	Text* statsName[Constants::NB_STATS];
	Text* stats[Constants::NB_STATS];

	void SetSelectedMove(int i);
	Panel* movesPanel;
	Button* movesButton[Constants::NB_MOVES_MAX_BY_POKE];
	int nbMoves;
	TextBlock* moveDetail;
	int selectedMove;

	Panel* descPanel;
	TextBlock* description;
};