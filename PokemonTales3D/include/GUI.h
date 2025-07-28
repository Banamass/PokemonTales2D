#pragma once

#include <unordered_map>
#include <map>
#include "Window.h"
#include "SharedTypes.h"
#include "Font.h"
#include "Shape.h"
#include "Pokemon.h"

/* Warning : memory usage of objects contained in the panel is managed by the panel
Usage of AddObj functions : AddObj("name", new Obj(param, ...))
Position of objects are managed by the panel*/
class Panel : public DrawableStatic {
public:
	Panel(ShaderManager* l_shaderMgr);
	Panel(ShaderManager* l_shaderMgr, glm::vec2 l_pos);
	virtual ~Panel();

	virtual void Draw(glm::mat4& cameraMatrix);

	/*Add a new element in the panel, return this element*/
	DrawableStatic* AddElement(DrawableStatic* elem, int zindex = 0);
	void DeleteElement(DrawableStatic* elem);

	virtual void SetPos(glm::vec2 l_pos);
	virtual void SetOffset(glm::vec2 l_offset);

protected:
	void UpdateElementsOffset();

	ShaderManager* shaderMgr;

	//Elements stored with their z index
	std::map<int, std::vector<DrawableStatic*>> elements; 
};

class TextField : public Panel {
public:
	TextField(glm::vec2 l_backSize, unsigned int l_fontSize
		, glm::vec2 l_pos, ShaderManager* l_shaderMgr);
	virtual ~TextField();

	void SetPadding(glm::vec2 l_padding);

	void AddMessage(const std::string& mess);

protected:
	void SetLinesPosition();

	ShaderManager* shaderMgr;

	unsigned int fontSize;
	float lineSpacing;
	Font* font;
	glm::vec2 backSize;
	glm::vec2 padding;

	RectangleShape* textBox;
	std::vector<Text*> lines;
};

class PokemonMoveBar : public Panel {
public:
	PokemonMoveBar(Font* l_font, ShaderManager* l_shaderMgr, glm::ivec2 l_pos);
	virtual ~PokemonMoveBar();

	virtual void Draw(glm::mat4& cameraMatrix);

	void SetPokemonMove(PokemonMove* l_move);
	glm::vec2 GetSize();

private:
	PokemonMove* move;

	glm::vec2 size;
	Text* moveName;
	Text* ppText;
	Text* powerText;
	RectangleShape* frame;
};

class PokemonStatsBar : public Panel {
public:
	PokemonStatsBar(Pokemon* l_poke, Font* l_font, ShaderManager* shaderMgr);
	virtual ~PokemonStatsBar();

	void Update(double dt);
	virtual void Draw(glm::mat4& cameraMatrix);

	void SetPokemon(Pokemon* poke);

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
};

class PokemonGUI : public Panel {
public:
	PokemonGUI(Pokemon* l_poke, Font* l_font, ShaderManager* shaderMgr);
	virtual ~PokemonGUI();

	void Update(double dt);
	virtual void Draw(glm::mat4& cameraMatrix);

	void SetPokemon(Pokemon* poke);
	Pokemon* GetPokemon();

private:
	Pokemon* poke;

	PokemonStatsBar* statsBar;
	Panel* movesBar;
	PokemonMoveBar* moveBars[4];
};

class GUI {
public:
	GUI(SharedContext* l_context);

	void Update(double dt);
	void Render();

	TextField* GetGameInfosField();

	void SetHoverPokemon(Pokemon* poke);
	void UnsetHoverPokemon();
	void SetSelectedPokemon(Pokemon* poke);
	void UnsetSelectedPokemon();

private:
	SharedContext* context;
	Font font;

	Panel gameName;

	PokemonStatsBar hoverPokeBar;

	PokemonGUI selectedPokeGUI;

	RectangleShape cursor;
	TextField gameInfos;
};