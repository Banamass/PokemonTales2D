#pragma once

#include <unordered_map>
#include <map>
#include "Window.h"
#include "SharedTypes.h"
#include "Font.h"
#include "Shape.h"
#include "Pokemon.h"

//Pure virtual class representing an objet that can be clickable
class Clickable {
public:
	Clickable();
	virtual ~Clickable() {};

	//Update the state of the clickable object
	virtual void Update(Window* win);

	//Get if the object is clicked
	bool GetClick();
	//Get if the object is pressed
	bool GetPress();
	//Get if the object is hovered
	bool GetHover();
	
	//Set the activation of the clickable object
	void SetActivated(bool b);
	//Get if the clickable object is activated
	bool GetActivated();

	//Get either the mouse pos is in the clickable object
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

class Scrollable {
public:
	Scrollable();
	virtual ~Scrollable();

	//Scroll the object
	virtual void Scroll(int xoffset, int yoffset) = 0;

protected:
	void UpdateScrollPos(int xoffset, int yoffset);

	int prevScrollXPos;
	int scrollXPos;

	int prevScrollYPos;
	int scrollYPos;
};

/* Warning : memory usage of objects contained in the panel is managed by the panel
Usage of AddElement function : AddElement(new DrawableStatic(param, ...), zindex)
Offset of objects are managed by the panel*/
class Panel : public DrawableStatic {
public:
	Panel(ShaderManager* l_shaderMgr);
	Panel(ShaderManager* l_shaderMgr, glm::vec2 l_pos);
	virtual ~Panel();

	//Draw the panel and its composants
	virtual void Draw(glm::mat4& cameraMatrix);

	/*Add a new element in the panel, return this element*/
	DrawableStatic* AddElement(DrawableStatic* elem, int zindex = 0);
	//Delete an element from the panel
	void DeleteElement(DrawableStatic* elem);

	//Set if an element is activated or not
	void SetActivatedElement(DrawableStatic* elem, bool b);
	//Get if an element is activated or not
	bool GetActivatedElement(DrawableStatic* elem);

	//Calculate the float rect of the panel, do not include unactivated elements
	virtual FloatRect GetFloatRect();

protected:
	struct PanelElement {
		PanelElement(DrawableStatic* d, bool a) 
			: drawable(d), activated(a){}
		DrawableStatic* drawable;
		bool activated;
	};

	PanelElement* GetPanelElem(DrawableStatic* elem);

	void UpdateElementsOffset();

	ShaderManager* shaderMgr;

	//Elements stored with their z index
	std::map<int, std::vector<PanelElement>> elements;
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
	//Set the color of the frame by default
	void SetFrameColor(glm::vec4 color);
	//Set the color of the frame when it's hovered
	void SetFrameHoverColor(glm::vec4 color);
	//Set the color of the frame when it's pressed
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

	//Set the padding between the frame and the text
	void SetPadding(glm::vec2 l_padding);

	//Clear all messages
	void Reset();

	//Add a message in the text field
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

class SelectBox : public Panel, public Clickable, public Scrollable{
public:
	SelectBox(Font* l_font, ShaderManager* l_shaderMgr
		, glm::vec2 size, int l_listSize, std::string defaultField = "");
	SelectBox(Font* l_font, ShaderManager* l_shaderMgr, const std::vector<std::string>& fields
		, glm::vec2 size, int l_listSize, std::string defaultField = "");
	virtual ~SelectBox();

	//Update the state of the selectBox
	virtual void Update(Window* win);
	virtual void Draw(glm::mat4& cameraMatrix);
	
	virtual void Scroll(int xoffset, int yoffset);

	//Add a new field to the select box
	virtual void AddField(const std::string& field);

	virtual bool In(glm::vec2 mousePos);

	//Reset the value of the selected field to the default field
	void ResetSelectedField();
	//Set the value of the selected field to a given field, do nothing if that field doesn't exists
	void SetSelectedField(const std::string& field);
	//Get the current selected field
	std::string GetSelectedField();
	//Get if the select box is in selection
	bool GetIsInSelection();

protected:
	void Setup(const std::vector<std::string>& fields, std::string defaultField);

	//Set the selected box to 'box', which can be equal to nullptr
	void SetSelectedBox(Button* box);
	
	//Set the selection state of the box
	void SetInSelection(bool b);

	/*Set the interval of activated button, [first, last[
	Return if the operation succeeded*/
	bool SetActButtonsInt(int first, int last);
	/*Modify the interval of activated button, shifting it by shift
	Rq : doing as much shift as possible
	Return if the operation succeeded*/
	//bool ShiftActButtonsInt(int shift);
	/*Modify the interval of activated button, shifting it by 1 in the provided direction
	Return if the operation succeeded*/
	bool ShiftActButtonsInt(Direction dir);
	/*Update the buttons state when a scroll occured*/
	void UpdateButtonsScroll();

	Font* font;
	ShaderManager* shaderMgr;

	glm::vec4 textColor;
	glm::vec4 frameColor;
	glm::vec2 bSize;
	int listSize;
	float charSize;
	std::string defaultField;

	float reduc;
	glm::vec2 panelPadding;
	bool isInSelection;

	Button* selectedBox; //Pointer to the selected box (part of boxes)
	//An interval of boxes attribut [first, second[ representing activated buttons
	std::pair<std::vector<Button*>::iterator, std::vector<Button*>::iterator> activButtonsInt;

	Panel* boxesPanel;
	Button* mainBox; //The box representing the text of the selected box
	std::vector<Button*> boxes;
	RectangleShape* panelFrame;
};

class PokemonMoveBar : public Panel, public Clickable {
public:
	PokemonMoveBar(Font* l_font, ShaderManager* l_shaderMgr, glm::ivec2 l_pos);
	virtual ~PokemonMoveBar();

	//Draw the move bar
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