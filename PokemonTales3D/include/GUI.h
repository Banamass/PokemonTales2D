#pragma once

#include <unordered_map>
#include <map>
#include "Window.h"
#include "SharedTypes.h"
#include "Font.h"
#include "Shape.h"
#include "Pokemon.h"

class GUIWidget {
public:
	GUIWidget() : pos(0.0f, 0.0f){}
	virtual ~GUIWidget(){}

	virtual void Render(Window* win) = 0;

	virtual void SetPos(glm::vec2 l_pos) { pos = l_pos; }
	virtual glm::vec2 GetPos() { return pos; }

protected:
	glm::vec2 pos;
};

/* Warning : memory usage of objects contained in the panel is managed by the panel
Usage of AddObj functions : AddObj("name", new Obj(param, ...))
Position of objects are managed by the panel*/
class Panel {
public:
	Panel(ShaderManager* l_shaderMgr);
	Panel(ShaderManager* l_shaderMgr, glm::vec2 l_pos);
	virtual ~Panel();

	virtual void Render(Window* win);

	/*Add a new shape in the panel, return this shape, or nullptr if the name already exist*/
	Shape* AddShape(const std::string& name, Shape* shape, int zindex = 0);
	/*Add a new widget in the panel, return this widget, or nullptr if the name already exist*/
	GUIWidget* AddWidget(const std::string& name, GUIWidget* widget);

	Shape* GetShape(const std::string& name);
	GUIWidget* GetWidgets(const std::string& name);

	virtual void SetPosition(glm::vec2 pos);

protected:
	void AddZindex(Shape* shape, int zindex);

	ShaderManager* shaderMgr;

	glm::vec2 pos;

	std::unordered_map<std::string, Shape*> shapes;
	std::unordered_map<std::string, GUIWidget*> widgets;

	std::map<int, std::vector<Shape*>> shapesZIndex;
};

class GUIText : public GUIWidget {
public:
	GUIText(Font* l_font, std::string l_text, Shader* l_shader);
	~GUIText();

	virtual void Render(Window* win);

	virtual void SetPos(glm::vec2 pos) {
		text.SetPos(pos);
	}
	virtual glm::vec2 GetPos() {
		return text.GetPos();
	}

	Text* GetText() { return &text; }

protected:
	Text text;
};

class TextField : public GUIWidget {
public:
	TextField(glm::vec2 l_backSize, unsigned int l_fontSize
		, glm::vec2 l_pos, ShaderManager* l_shaderMgr);
	~TextField();

	void SetPadding(glm::vec2 l_padding);
	virtual void SetPos(glm::vec2);

	void AddMessage(const std::string& mess);

	virtual void Render(Window* win);

protected:
	void SetLinesPosition();

	ShaderManager* shaderMgr;

	unsigned int fontSize;
	float lineSpacing;
	Font* font;
	glm::vec2 backSize;
	glm::vec2 padding;

	RectangleShape textBox;
	std::vector<Text> lines;
};

class PokemonMoveBar : public GUIWidget {
public:
	PokemonMoveBar(Font* l_font, ShaderManager* l_shaderMgr, glm::ivec2 l_pos);
	virtual ~PokemonMoveBar();

	virtual void Render(Window* win);

	void SetPokemonMove(PokemonMove* l_move);
	virtual void SetPos(glm::vec2 l_pos);
	glm::vec2 GetSize();

private:
	PokemonMove* move;

	glm::vec2 size;
	Panel panel;
	GUIText* moveName;
	GUIText* ppText;
	GUIText* powerText;
	RectangleShape* frame;
};

class PokemonStatsBar : public GUIWidget {
public:
	PokemonStatsBar(Pokemon* l_poke, Font* l_font, ShaderManager* shaderMgr);
	virtual ~PokemonStatsBar();

	void Update(double dt);
	virtual void Render(Window* win);

	void SetPokemon(Pokemon* poke);
	virtual void SetPos(glm::vec2 pos);

	glm::vec2 GetSize();
	virtual glm::vec2 GetPos();
	Pokemon* GetPokemon();

protected:
	Font* font;

	Pokemon* poke;

	glm::vec2 size;
	Panel panel;
	GUIText* pokeName;
	GUIText* healthText;

	glm::vec2 healthBarSize;
	glm::vec2 healthBarPos;
	RectangleShape* healthBar;
	RectangleShape* healthBarFrame;
	RectangleShape* healthBarBack;
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

	Panel selectedPokeInfos;
	PokemonMoveBar* moveBars[4];
	RectangleShape* moveBarsFrame;
	PokemonStatsBar* selectedPokeBar;


	Model cursorModel;
	TextField gameInfos;
	RectangleShape rect;
};