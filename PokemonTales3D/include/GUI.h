#pragma once

#include <unordered_map>
#include <map>
#include "Window.h"
#include "SharedTypes.h"
#include "Font.h"
#include "Shape.h"
#include "Pokemon.h"

class Clickable {
public:
	Clickable();
	virtual ~Clickable() {};

	void Update(Window* win);

	bool GetClick();
	bool GetPress();
	bool GetHover();

protected:
	virtual bool In(glm::vec2 mousePos) = 0;

	virtual void Press() {}
	virtual void Hover() {}
	virtual void Click() {}

	virtual void UnPress() {}
	virtual void UnHover() {}
	virtual void UnClick() {}

	void SetPress(bool b);
	void SetHover(bool b);
	void SetClick(bool b);

	bool click;
	bool press;
	bool hover;
};

/* Warning : memory usage of objects contained in the panel is managed by the panel
Usage of AddElement function : AddElement(new DrawableStatic(param, ...), zindex)
Offset of objects are managed by the panel*/
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

class Button : public Panel, public Clickable {
public:
	Button(Font* l_font, ShaderManager* l_shaderMgr, glm::ivec2 l_pos);
	virtual ~Button();

	virtual void SetPos(glm::vec2 l_pos);
	void SetOrigin(Location l_origin);
	void SetSize(glm::vec2 l_size);
	void SetCharacterSize(float l_charSize);
	void SetText(std::string text);
	void SetTextColor(glm::vec4 color);
	void SetFrameColor(glm::vec4 color);
	void SetFrameHoverColor(glm::vec4 color);
	void SetFramePressColor(glm::vec4 color);

	virtual void Hover();
	virtual void UnHover();

	virtual void Press();
	virtual void UnPress();

protected:
	virtual bool In(glm::vec2 mousePos);

	RectangleShape* frame;
	Text* text;

	float characterSize;
	glm::vec2 size;
	Location origin;

	glm::vec4 color;
	glm::vec4 hoverColor;
	glm::vec4 pressColor;
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

class PokemonMoveBar : public Panel, public Clickable {
public:
	PokemonMoveBar(Font* l_font, ShaderManager* l_shaderMgr, glm::ivec2 l_pos);
	virtual ~PokemonMoveBar();

	virtual void Draw(glm::mat4& cameraMatrix);

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
	Text* typeText;
	RectangleShape* typeFrame;
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
	float simulatedDamages;
};