#include "GUI.h"

/*---------------GUIText---------------*/

GUIText::GUIText(Font* l_font, std::string l_text, Shader* l_shader) : text(l_font, l_text, l_shader)
{}
GUIText::~GUIText(){}

void GUIText::Render(Window* win) {
	win->DrawStatic(&text);
}

/*---------------TextField---------------*/

TextField::TextField(glm::vec2 l_backSize, unsigned int l_fontSize, glm::vec2 l_pos, ShaderManager* l_shaderMgr)
	: backSize(l_backSize), fontSize(l_fontSize),
	shaderMgr(l_shaderMgr), textBox(l_shaderMgr->GetShader("SimpleShader"))
{
	pos = l_pos;

	textBox.SetPos(pos);
	textBox.SetSize(l_backSize);
	textBox.SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));

	font = new Font("Resources\\fonts\\arial.ttf");

	padding = glm::vec2(0, 0);
	lineSpacing = 1.2 * fontSize;
}
TextField::~TextField() {
	delete font;
}

void TextField::AddMessage(const std::string& mess) {
	//Text& itr = messages.emplace_back(font, mess, shaderMgr->GetShader("FontShader"));
	
	if (mess == "")
		return;
	float maxTextWidth = backSize.x - padding.x * 2;
	Text& itr = lines.emplace_back(font, "", shaderMgr->GetShader("FontShader"));
	itr.SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	itr.SetCharacterSize(fontSize);
	unsigned int nbCharacter = 0;
	while (itr.GetFloatRect().size.x < maxTextWidth && nbCharacter < mess.size()) {
		itr.AddText(std::string(1, mess[nbCharacter]));
		nbCharacter++;
	}
	if (itr.GetFloatRect().size.x < maxTextWidth) {
		SetLinesPosition();
		return;
	}
	itr.RemoveText(1);

	int spaceInd = nbCharacter - 1;
	for(; spaceInd >= 0 && mess[spaceInd] != ' '; spaceInd--){}
	if (spaceInd == -1)
		spaceInd = nbCharacter - 1;

	itr.RemoveText(nbCharacter - spaceInd - 1);

	SetLinesPosition();
	AddMessage(mess.substr(spaceInd + 1, mess.size() - spaceInd));

	/*FloatRect hb = itr.GetFloatRect();
	textBox.SetPos(hb.pos);
	textBox.SetSize(hb.size);*/
}

void TextField::Render(Window* win) {
	win->DrawStatic(&textBox);
	for (auto& mess : lines) {
		win->DrawStatic(&mess);
	}
}

void TextField::SetLinesPosition(){
	while (lines.size() * lineSpacing > textBox.GetSize().y - 2 * padding.y)
		lines.erase(lines.begin());

	glm::vec2 posTextTopLeft(pos.x + padding.x, pos.y + backSize.y - padding.y);
	posTextTopLeft.y -= lineSpacing;
	for (auto& line : lines) {
		line.SetPos(posTextTopLeft);
		posTextTopLeft.y -= lineSpacing;
	}
}

void TextField::SetPadding(glm::vec2 l_padding) {
	padding = l_padding;
	SetLinesPosition();
}

void TextField::SetPos(glm::vec2 l_pos) {
	pos = l_pos;
	SetLinesPosition();
}

/*---------------PokemonStatsBar---------------*/

PokemonStatsBar::PokemonStatsBar(Pokemon* l_poke, Font* l_font, ShaderManager* shaderMgr)
	: poke(l_poke), font(l_font), panel(shaderMgr)
{
	size = glm::vec2(250.0f, 100.0f);

	pokeName = (GUIText*) panel.AddWidget("PokeName", new GUIText(font, "", shaderMgr->GetShader("FontShader")));
	pokeName->GetText()->SetCharacterSize(20.0f);
	pokeName->SetPos(glm::vec2(10.0, size.y - 25.0f));
	pokeName->GetText()->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));

	RectangleShape* frame = (RectangleShape*)panel.AddShape("Frame", 
		new RectangleShape(size, shaderMgr->GetShader("SimpleShader")));
	frame->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));

	if (poke != nullptr)
		pokeName->GetText()->SetText(poke->GetName());
	else
		pokeName->GetText()->SetText("None");
}
PokemonStatsBar::~PokemonStatsBar() {

}

void PokemonStatsBar::Render(Window* win) {
	if (poke != nullptr) {
		panel.Render(win);
	}
}

void PokemonStatsBar::SetPokemon(Pokemon* l_poke) {
	poke = l_poke;
	if (poke == nullptr)
		return;
	pokeName->GetText()->SetText(poke->GetName());
}

void PokemonStatsBar::SetPos(glm::vec2 l_pos) {
	pos = l_pos;
	panel.SetPosition(pos);
}

glm::vec2 PokemonStatsBar::GetSize() { return size; }
glm::vec2 PokemonStatsBar::GetPos() { return pos; }
Pokemon* PokemonStatsBar::GetPokemon() { return poke; }

/*---------------Panel---------------*/

Panel::Panel(ShaderManager* l_shaderMgr) 
	: shaderMgr(l_shaderMgr), pos(0.0f, 0.0f) 
{}
Panel::Panel(ShaderManager* l_shaderMgr, glm::vec2 l_pos)
	: shaderMgr(l_shaderMgr), pos(l_pos) 
{}

Panel::~Panel() {
	for (auto& itr : widgets)
		delete itr.second;
	for (auto& itr : shapes)
		delete itr.second;
}

void Panel::Render(Window* win) {
	for (auto& itr : shapes) {
		glm::vec2 temp = itr.second->GetPos();
		itr.second->SetPos(temp + pos);
		itr.second->Draw();
		itr.second->SetPos(temp);
	}
	for (auto& itr : widgets) {
		glm::vec2 temp = itr.second->GetPos();
		itr.second->SetPos(temp + pos);
		itr.second->Render(win);
		itr.second->SetPos(temp);
	}
}

Shape* Panel::AddShape(const std::string& name, Shape* shape) {
	auto itr = shapes.find(name);
	if (itr != shapes.end())
		return nullptr;
	shapes.emplace(name, shape);
	return shape;
}
GUIWidget* Panel::AddWidget(const std::string& name, GUIWidget* widget) {
	auto itr = widgets.find(name);
	if (itr != widgets.end())
		return nullptr;
	widgets.emplace(name, widget);
	return widget;
}

Shape* Panel::GetShape(const std::string& name) {
	auto itr = shapes.find(name);
	if (itr == shapes.end())
		return nullptr;
	return itr->second;
}
GUIWidget* Panel::GetWidgets(const std::string& name) {
	auto itr = widgets.find(name);
	if (itr == widgets.end())
		return nullptr;
	return itr->second;
}

void Panel::SetPosition(glm::vec2 l_pos) {
	pos = l_pos;
}

/*---------------GUI---------------*/

GUI::GUI(SharedContext* l_context)
	: context(l_context), cursorModel("Resources\\Box\\box.obj"),
	gameInfos(glm::vec2(175, 200), 14, glm::vec2(10,350), l_context->shaderManager),
	rect(glm::vec2(100.0f, 100.0f), l_context->shaderManager->GetShader("SimpleShader")),
	gameName(l_context->shaderManager),
	font("Resources\\fonts\\arial.ttf", 40.0f),
	hoverPokeBar(nullptr, &font, context->shaderManager),
	selectedPokeBar(nullptr, &font, context->shaderManager)
{
	context->gui = this;

	//GameInfos
	gameInfos.SetPadding(glm::vec2(10.0f, 4.0f));
	
	//Cursor
	rect.SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	rect.SetSize(glm::vec2(10.0f, 10.0f));
	rect.SetPos(glm::vec2(Constants::WIN_WIDTH / 2, Constants::WIN_HEIGHT / 2));
	rect.SetOrigin(Location::Middle);

	//Game Name
	GUIText* text = (GUIText*)gameName.AddWidget("GameName",
		new GUIText(&font, "PokeTales", context->shaderManager->GetShader("FontShader")));
	text->SetPos(glm::vec2(5.0f, 5.0f));
	text->GetText()->SetCharacterSize(40.0f);

	glm::vec2 gameNameSize = text->GetText()->GetFloatRect().size + glm::vec2(10.0f, 10.0f);
	gameName.SetPosition(glm::vec2(0, Constants::WIN_HEIGHT - gameNameSize.y));

	RectangleShape* frame = (RectangleShape*) gameName.AddShape("BackFrame", 
		new RectangleShape(gameNameSize, context->shaderManager->GetShader("SimpleShader")));
	frame->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));

	//Stats Bar
	hoverPokeBar.SetPos(glm::vec2(Constants::WIN_WIDTH - hoverPokeBar.GetSize().x, Constants::WIN_HEIGHT - hoverPokeBar.GetSize().y));
	selectedPokeBar.SetPos(glm::vec2(225.0f, Constants::WIN_HEIGHT - hoverPokeBar.GetSize().y));
}

void GUI::Render() {
	glDisable(GL_DEPTH_TEST);

	context->win->DrawStatic(&rect);
	gameInfos.Render(context->win);
	gameName.Render(context->win);
	if (selectedPokeBar.GetPokemon() != hoverPokeBar.GetPokemon())
		hoverPokeBar.Render(context->win);
	selectedPokeBar.Render(context->win);

	glEnable(GL_DEPTH_TEST);
}

void GUI::SetHoverPokemon(Pokemon* poke) {
	hoverPokeBar.SetPokemon(poke);
}
void GUI::UnsetHoverPokemon() {
	hoverPokeBar.SetPokemon(nullptr);
}

void GUI::SetSelectedPokemon(Pokemon* poke) {
	selectedPokeBar.SetPokemon(poke);
}
void GUI::UnsetSelectedPokemon() {
	selectedPokeBar.SetPokemon(nullptr);
}

TextField* GUI::GetGameInfosField() { return &gameInfos; }