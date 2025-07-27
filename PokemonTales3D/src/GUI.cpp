#include "GUI.h"

/*---------------GUIText---------------*/

GUIText::GUIText(Font* l_font, std::string l_text, Shader* l_shader) : text(l_font, l_text, l_shader)
{}
GUIText::~GUIText(){}

void GUIText::Render(Window* win) {
	win->DrawStatic(&text);
}

/*---------------Panel---------------*/

Panel::Panel(ShaderManager* l_shaderMgr)
	: shaderMgr(l_shaderMgr), pos(0.0f, 0.0f)
{
}
Panel::Panel(ShaderManager* l_shaderMgr, glm::vec2 l_pos)
	: shaderMgr(l_shaderMgr), pos(l_pos)
{
}

Panel::~Panel() {
	for (auto& itr : widgets)
		delete itr.second;
	for (auto& itr : shapes)
		delete itr.second;
}

void Panel::Render(Window* win) {
	for (auto& shapes : shapesZIndex) {
		for (auto& shape : shapes.second) {
			glm::vec2 temp = shape->GetPos();
			shape->SetPos(temp + pos);
			shape->Draw();
			shape->SetPos(temp);
		}
	}
	for (auto& itr : widgets) {
		glm::vec2 temp = itr.second->GetPos();
		itr.second->SetPos(temp + pos);
		itr.second->Render(win);
		itr.second->SetPos(temp);
	}
}

Shape* Panel::AddShape(const std::string& name, Shape* shape, int zindex) {
	auto itr = shapes.find(name);
	if (itr != shapes.end())
		return nullptr;
	shapes.emplace(name, shape);
	AddZindex(shape, zindex);
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

void Panel::AddZindex(Shape* shape, int zindex) {
	if (shapesZIndex.find(zindex) == shapesZIndex.end()) {
		shapesZIndex.emplace(zindex, std::vector<Shape*>({ shape }));
		return;
	}
	shapesZIndex[zindex].push_back(shape);
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

/*PokemonMoveBar*/

PokemonMoveBar::PokemonMoveBar(Font* l_font, ShaderManager* l_shaderMgr, glm::ivec2 l_pos)
	: panel(l_shaderMgr) {
	pos = l_pos;
	panel.SetPosition(pos);
	move = nullptr;
	size = glm::vec2(150, 50);
	float charSize = 14.0f;
	float margin = 5.0f;

	ppText = (GUIText*)panel.AddWidget("ppText", new GUIText(l_font, "10/10", l_shaderMgr->GetShader("FontShader")));
	ppText->GetText()->SetColor(glm::vec3(0.0f));
	ppText->GetText()->SetCharacterSize(charSize);
	ppText->SetPos(glm::vec2(size.x - 50, size.y - charSize - margin));

	moveName = (GUIText*)panel.AddWidget("moveName", new GUIText(l_font, "Move", l_shaderMgr->GetShader("FontShader")));
	moveName->GetText()->SetColor(glm::vec3(0.0f));
	moveName->GetText()->SetCharacterSize(charSize);
	moveName->SetPos(glm::vec2(margin, size.y - charSize - margin));

	powerText = (GUIText*)panel.AddWidget("powerText", new GUIText(l_font, "Power : 50", l_shaderMgr->GetShader("FontShader")));
	powerText->GetText()->SetColor(glm::vec3(0.0f));
	powerText->GetText()->SetCharacterSize(charSize);
	powerText->SetPos(glm::vec2(margin, margin));

	frame = (RectangleShape*)panel.AddShape("frame", new RectangleShape(size, l_shaderMgr->GetShader("SimpleShader")), -1);
	frame->SetColor(glm::vec4(1.0f));
}
PokemonMoveBar::~PokemonMoveBar() {

}

void PokemonMoveBar::Render(Window* win) {
	if(move != nullptr)
		panel.Render(win);
}

void PokemonMoveBar::SetPokemonMove(PokemonMove* l_move) {
	move = l_move;
	if (move == nullptr)
		return;
	moveName->GetText()->SetText(move->data->name);
	ppText->GetText()->SetText(std::to_string(move->pp) + "/" + std::to_string(move->data->pp));
	powerText->GetText()->SetText(std::to_string(move->data->power));
}

void PokemonMoveBar::SetPos(glm::vec2 l_pos) {
	pos = l_pos;
	panel.SetPosition(pos);
}

glm::vec2 PokemonMoveBar::GetSize() {
	return size;
}

/*---------------PokemonStatsBar---------------*/

PokemonStatsBar::PokemonStatsBar(Pokemon* l_poke, Font* l_font, ShaderManager* shaderMgr)
	: poke(l_poke), font(l_font), panel(shaderMgr)
{
	size = glm::vec2(250.0f, 100.0f);
	healthBarSize = glm::vec2(200.0f, 30.0f);
	healthBarPos = glm::vec2(10.0f, 10.0f);
	glm::vec2 healthBarFrameOffset(3.0f, 3.0f);

	//HealthBar
	healthText = (GUIText*)panel.AddWidget("PokeHealth", new GUIText(font, "", shaderMgr->GetShader("FontShader")));
	healthText->GetText()->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
	healthText->GetText()->SetCharacterSize(15.0f);
	healthText->SetPos(glm::vec2(10.0f, 50.0f));

	healthBar = (RectangleShape*)panel.AddShape("HealthBar", new RectangleShape(healthBarSize, shaderMgr->GetShader("SimpleShader")), 3);
	healthBar->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	healthBar->SetPos(healthBarPos);

	healthBarBack = (RectangleShape*)panel.AddShape("HealthBarBack", new RectangleShape(healthBarSize, shaderMgr->GetShader("SimpleShader")), 2);
	healthBarBack->SetColor(glm::vec4(1.0f));
	healthBarBack->SetPos(healthBarPos);

	healthBarFrame = (RectangleShape*)panel.AddShape("HealthBarFrame", new RectangleShape(healthBarSize + 2.0f*healthBarFrameOffset, shaderMgr->GetShader("SimpleShader")), 1);
	healthBarFrame->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
	healthBarFrame->SetPos(healthBarPos - healthBarFrameOffset);

	//Name
	pokeName = (GUIText*) panel.AddWidget("PokeName", new GUIText(font, "", shaderMgr->GetShader("FontShader")));
	pokeName->GetText()->SetCharacterSize(20.0f);
	pokeName->SetPos(glm::vec2(10.0, size.y - 25.0f));
	pokeName->GetText()->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));

	RectangleShape* frame = (RectangleShape*)panel.AddShape("Frame", 
		new RectangleShape(size, shaderMgr->GetShader("SimpleShader")), -1);
	frame->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));

	if (poke != nullptr)
		pokeName->GetText()->SetText(poke->GetName());
	else
		pokeName->GetText()->SetText("None");
	
}
PokemonStatsBar::~PokemonStatsBar() {

}

void PokemonStatsBar::Update(double dt) {
	if (poke != nullptr) {
		healthText->GetText()->SetText("PV : " + std::to_string((int)poke->GetHealth()));
		healthBar->SetSize(glm::vec2((poke->GetHealth() / poke->GetMaxHealth()) * healthBarSize.x, healthBarSize.y));
	}
}

void PokemonStatsBar::Render(Window* win) {
	panel.Render(win);
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

/*---------------GUI---------------*/

GUI::GUI(SharedContext* l_context)
	: context(l_context), cursorModel("Resources\\Box\\box.obj"),
	gameInfos(glm::vec2(175, 200), 14, glm::vec2(10,350), l_context->shaderManager),
	rect(glm::vec2(100.0f, 100.0f), l_context->shaderManager->GetShader("SimpleShader")),
	gameName(l_context->shaderManager),
	font("Resources\\fonts\\arial.ttf", 40.0f),
	hoverPokeBar(nullptr, &font, context->shaderManager),
	selectedPokeInfos(context->shaderManager)
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

	//Hove pokemons stats bar
	hoverPokeBar.SetPos(glm::vec2(Constants::WIN_WIDTH - hoverPokeBar.GetSize().x, Constants::WIN_HEIGHT - hoverPokeBar.GetSize().y));

	//Selected pokemon infos
	selectedPokeBar = (PokemonStatsBar*)selectedPokeInfos.AddWidget("stats bar", new PokemonStatsBar(nullptr, &font, context->shaderManager));
	selectedPokeBar->SetPos(glm::vec2(0, 0));

	//Moves
	glm::vec2 moveBarPadding(30.0f, 15.0f);
	glm::vec2 moveBarFramePadding(20.0f, 20.0f);
	glm::vec2 moveBarsPos(selectedPokeBar->GetSize().x + moveBarPadding.x + moveBarFramePadding.x, -100.0f);
	for (int i = 0; i < 4; i++) {
		moveBars[i] = (PokemonMoveBar*)selectedPokeInfos.AddWidget("moveBar" + std::to_string(i), new PokemonMoveBar(&font, context->shaderManager, glm::vec2(0, 0)));
		glm::vec2 size = moveBars[i]->GetSize();
		if (i == 0)
			moveBars[i]->SetPos(moveBarsPos + glm::vec2(0, 0));
		if (i == 1)
			moveBars[i]->SetPos(moveBarsPos + glm::vec2(size.x + moveBarPadding.x, 0));
		if (i == 2)
			moveBars[i]->SetPos(moveBarsPos + glm::vec2(0, size.y + moveBarPadding.y));
		if (i == 3)
			moveBars[i]->SetPos(moveBarsPos + glm::vec2(size.x + moveBarPadding.x, size.y + moveBarPadding.y));
	}

	moveBarsFrame = (RectangleShape*)selectedPokeInfos.AddShape("moveBarsFrame",
		new RectangleShape(glm::vec2(2*moveBarFramePadding.x + moveBars[0]->GetSize().x*2 + moveBarPadding.x,
									2*moveBarFramePadding.y + moveBars[0]->GetSize().y*2 + moveBarPadding.y),
			context->shaderManager->GetShader("SimpleShader")), -1);
	moveBarsFrame->SetPos(moveBarsPos - moveBarFramePadding);
	moveBarsFrame->SetColor(glm::vec4(glm::vec3(1.0f), 0.5f));

	selectedPokeInfos.SetPosition(glm::vec2(225.0f, Constants::WIN_HEIGHT - selectedPokeBar->GetSize().y));
}

void GUI::Update(double dt) {
	selectedPokeBar->Update(dt);
	hoverPokeBar.Update(dt);
}

void GUI::Render() {
	glDisable(GL_DEPTH_TEST);

	context->win->DrawStatic(&rect);
	gameInfos.Render(context->win);
	gameName.Render(context->win);
	if (selectedPokeBar->GetPokemon() != hoverPokeBar.GetPokemon() &&
		hoverPokeBar.GetPokemon() != nullptr)
		hoverPokeBar.Render(context->win);
	if(selectedPokeBar->GetPokemon() != nullptr)
		selectedPokeInfos.Render(context->win);

	glEnable(GL_DEPTH_TEST);
}

void GUI::SetHoverPokemon(Pokemon* poke) {
	hoverPokeBar.SetPokemon(poke);
}
void GUI::UnsetHoverPokemon() {
	hoverPokeBar.SetPokemon(nullptr);
}

void GUI::SetSelectedPokemon(Pokemon* poke) {
	selectedPokeBar->SetPokemon(poke);
	for (int i = 0; i < 4; i++) {
		if (PokemonMove* move = poke->GetMove(i))
			moveBars[i]->SetPokemonMove(move);
	}
}
void GUI::UnsetSelectedPokemon() {
	selectedPokeBar->SetPokemon(nullptr);
}

TextField* GUI::GetGameInfosField() { return &gameInfos; }