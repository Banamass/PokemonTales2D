#pragma once

#include "Window.h"
#include "SharedTypes.h"
#include "Font.h"
#include "Shape.h"

class TextField {
public:
	TextField(glm::vec2 l_backSize, unsigned int l_fontSize
		, glm::vec2 l_pos, ShaderManager* l_shaderMgr);
	~TextField();

	void SetPadding(glm::vec2 l_padding);

	void AddMessage(const std::string& mess);

	void Render(Window* win);

private:
	void SetLinesPosition();

	ShaderManager* shaderMgr;

	unsigned int fontSize;
	float lineSpacing;
	Font* font;
	glm::vec2 backSize;
	glm::vec2 padding;

	glm::vec2 pos;

	RectangleShape textBox;
	std::vector<Text> lines;
};

class GUI {
public:
	GUI(SharedContext* l_context);

	void Render();

private:
	SharedContext* context;

	Model cursorModel;
	TextField gameInfos;
	RectangleShape rect;
};