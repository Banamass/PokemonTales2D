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

	virtual void Update(Window* win);

	bool GetClick();
	bool GetPress();
	bool GetHover();
	
	void SetActivated(bool b);
	bool GetActivated();

	virtual bool In(glm::vec2 mousePos) = 0;

protected:

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

	bool activated;
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

	std::string GetText();

	virtual void Hover();
	virtual void UnHover();

	virtual void Press();
	virtual void UnPress();

	virtual bool In(glm::vec2 mousePos);

protected:
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

	void Reset();

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

class SelectBox : public Panel, public Clickable{
public:
	SelectBox(Font* l_font, ShaderManager* l_shaderMgr
		, glm::vec2 size, std::string defaultField = "");
	SelectBox(Font* l_font, ShaderManager* l_shaderMgr, const std::vector<std::string>& fields
		, glm::vec2 size, std::string defaultField = "");
	virtual ~SelectBox();

	virtual void Update(Window* win);
	virtual void Draw(glm::mat4& cameraMatrix);

	virtual void AddField(const std::string& field);

	virtual bool In(glm::vec2 mousePos);

	void ResetSelectedField();
	void SetSelectedField(const std::string& field);
	std::string GetSelectedField();

protected:
	void Setup(const std::vector<std::string>& fields, std::string defaultField);

	//Set the selected box to 'box', which can be equal to nullptr
	void SetSelectedBox(Button* box);

	Font* font;
	ShaderManager* shaderMgr;

	glm::vec4 textColor;
	glm::vec4 frameColor;
	glm::vec2 bSize;
	float charSize;
	std::string defaultField;

	float reduc;
	glm::vec2 panelPadding;
	Button* mainBox; //The box representing the text of the selected box
	Button* selectedBox; //Pointer to the selected box (part of boxes)
	bool isInSelection;
	Panel* boxesPanel;
	std::vector<Button*> boxes;
	RectangleShape* panelFrame;
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