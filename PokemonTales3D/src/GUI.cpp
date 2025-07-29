#include "GUI.h"
#include "GameSystem.h"

/*---------------Panel---------------*/

Panel::Panel(ShaderManager* l_shaderMgr)
	: DrawableStatic(), shaderMgr(l_shaderMgr){}
Panel::Panel(ShaderManager* l_shaderMgr, glm::vec2 l_pos)
	: DrawableStatic(l_pos), shaderMgr(l_shaderMgr)
{
}

Panel::~Panel() {
	for (auto& layer : elements)
		for (auto& elem : layer.second)
			delete elem;
}

void Panel::Draw(glm::mat4& cameraMatrix) {
	for (auto& itr : elements) {
		for (auto& elem : itr.second) {
			elem->Draw(cameraMatrix);
		}
	}
}

DrawableStatic* Panel::AddElement(DrawableStatic* elem, int zindex) {
	if (elements.find(zindex) == elements.end()) {
		elements.emplace(zindex, std::vector<DrawableStatic*>({ elem }));
	}
	else {
		elements[zindex].push_back(elem);
	}
	elem->SetOffset(pos + offset);
	return elem;
}

void Panel::DeleteElement(DrawableStatic* elem) {
	for (auto& layer : elements) {
		for (auto itr = layer.second.begin(); itr != layer.second.end(); itr++) {
			if (elem == *itr) {
				layer.second.erase(itr);
				return;
			}
		}
	}
}

void Panel::UpdateElementsOffset() {
	for (auto& layer : elements) 
		for(auto& elem : layer.second)
			elem->SetOffset(pos + offset);
}

void Panel::SetPos(glm::vec2 l_pos) {
	if (pos == l_pos)
		return;
	pos = l_pos;
	UpdateElementsOffset();
}

void Panel::SetOffset(glm::vec2 l_offset) {
	if (offset == l_offset)
		return;
	offset = l_offset;
	UpdateElementsOffset();
}

/*---------------TextField---------------*/

TextField::TextField(glm::vec2 l_backSize, unsigned int l_fontSize, glm::vec2 l_pos, ShaderManager* l_shaderMgr)
	: Panel(l_shaderMgr, l_pos), backSize(l_backSize), fontSize(l_fontSize), shaderMgr(l_shaderMgr)
{
	textBox = (RectangleShape*)AddElement(new RectangleShape(l_backSize, shaderMgr->GetShader("SimpleShader")), -1);

	textBox->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));

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
	if (maxTextWidth < 0.001f)
		return;
	Text* text = (Text*)AddElement(new Text(font, "", shaderMgr->GetShader("FontShader")));
	lines.push_back(text);
	text->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	text->SetCharacterSize(fontSize);
	unsigned int nbCharacter = 0;
	while (text->GetFloatRect().size.x < maxTextWidth && nbCharacter < mess.size()) {
		text->AddText(std::string(1, mess[nbCharacter]));
		nbCharacter++;
	}
	if (text->GetFloatRect().size.x < maxTextWidth) {
		SetLinesPosition();
		return;
	}
	text->RemoveText(1);

	int spaceInd = nbCharacter - 1;
	for(; spaceInd >= 0 && mess[spaceInd] != ' '; spaceInd--){}
	if (spaceInd == -1)
		spaceInd = nbCharacter - 1;

	text->RemoveText(nbCharacter - spaceInd - 1);

	SetLinesPosition();
	AddMessage(mess.substr(spaceInd + 1, mess.size() - spaceInd));

	/*FloatRect hb = itr.GetFloatRect();
	textBox.SetPos(hb.pos);
	textBox.SetSize(hb.size);*/
}

void TextField::SetLinesPosition(){
	while (lines.size() * lineSpacing > textBox->GetSize().y - 2 * padding.y) {
		DeleteElement(*(lines.begin()));
		lines.erase(lines.begin());
	}

	glm::vec2 posTextTopLeft(padding.x, backSize.y - padding.y);
	posTextTopLeft.y -= lineSpacing;
	for (auto& text : lines) {
		text->SetPos(posTextTopLeft);
		posTextTopLeft.y -= lineSpacing;
	}
}

void TextField::SetPadding(glm::vec2 l_padding) {
	padding = l_padding;
	SetLinesPosition();
}

/*---------------PokemonMoveBar---------------*/

PokemonMoveBar::PokemonMoveBar(Font* l_font, ShaderManager* l_shaderMgr, glm::ivec2 l_pos)
	: Panel(l_shaderMgr) {
	move = nullptr;
	size = glm::vec2(150, 50);
	charSize = 14.0f;
	float margin = 5.0f;

	isSelected = false;
	unselectedColor = glm::vec4(1.0f);
	selectedColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	ppText = (Text*)AddElement( new Text(l_font, "10/10", l_shaderMgr->GetShader("FontShader")));
	ppText->SetColor(glm::vec3(0.0f));
	ppText->SetCharacterSize(charSize);
	ppText->SetPos(glm::vec2(size.x - 50, margin));

	moveName = (Text*)AddElement(new Text(l_font, "Move", l_shaderMgr->GetShader("FontShader")));
	moveName->SetColor(glm::vec3(0.0f));
	moveName->SetCharacterSize(charSize);
	moveName->SetPos(glm::vec2(margin, size.y - charSize - margin));

	powerText = (Text*)AddElement(new Text(l_font, "Power : 50", l_shaderMgr->GetShader("FontShader")));
	powerText->SetColor(glm::vec3(0.0f));
	powerText->SetCharacterSize(charSize);
	powerText->SetPos(glm::vec2(margin, margin));

	typeText = (Text*)AddElement(new Text(l_font, "Type1", l_shaderMgr->GetShader("FontShader")));
	typeText->SetColor(glm::vec3(0.0f));
	typeText->SetCharacterSize(charSize);
	typeText->SetPos(glm::vec2(size.x - 50, size.y - charSize - margin));
	
	typeFramePadding = glm::vec2(2.0f, 2.0f);
	typeFrame = (RectangleShape*)AddElement(new RectangleShape(l_shaderMgr->GetShader("SimpleShader")), -1);
	typeFrame->SetColor(glm::vec4(0.0f));
	typeFrame->SetPos(glm::vec2(size.x - 50, size.y - charSize - margin) - typeFramePadding);

	frame = (RectangleShape*)AddElement(new RectangleShape(size, l_shaderMgr->GetShader("SimpleShader")), -2);
	frame->SetColor(unselectedColor);
}
PokemonMoveBar::~PokemonMoveBar() {

}

void PokemonMoveBar::Draw(glm::mat4& cameraMatrix){
	if (move != nullptr)
		Panel::Draw(cameraMatrix);
}

bool PokemonMoveBar::IsClicked(glm::vec2 mousePos) {
	if (move == nullptr)
		return false;
	glm::vec2 realMousePos = glm::vec2(mousePos.x , Constants::WIN_HEIGHT- mousePos.y);
	glm::vec2 realPos = offset + pos;
	FloatRect hitBox(realPos, size);
	return hitBox.Contains(realMousePos);
}

void PokemonMoveBar::SetSelect(bool b) {
	isSelected = b;
	frame->SetColor(isSelected ? selectedColor : unselectedColor);
}

void PokemonMoveBar::SetPokemonMove(PokemonMove* l_move) {
	move = l_move;
	if (move == nullptr)
		return;
	moveName->SetText(move->data->name);
	ppText->SetText(std::to_string(move->pp) + "/" + std::to_string(move->data->pp));
	powerText->SetText(std::to_string(move->data->power));
	typeText->SetText(GetStringFromPokeType(move->data->type));
	typeFrame->SetColor(GetColorFromPokeType(move->data->type));
	typeFrame->SetSize(glm::vec2(typeText->GetFloatRect().size.x, charSize) + 2.0f * typeFramePadding);
}

glm::vec2 PokemonMoveBar::GetSize() {
	return size;
}

/*---------------PokemonStatsBar---------------*/

PokemonStatsBar::PokemonStatsBar(Pokemon* l_poke, Font* l_font, ShaderManager* shaderMgr)
	: Panel(shaderMgr), poke(l_poke), font(l_font)
{
	size = glm::vec2(250.0f, 100.0f);
	healthBarSize = glm::vec2(200.0f, 30.0f);
	healthBarPos = glm::vec2(10.0f, 10.0f);
	glm::vec2 healthBarFrameOffset(3.0f, 3.0f);

	//HealthBar
	healthText = (Text*) AddElement( new Text(font, "", shaderMgr->GetShader("FontShader")));
	healthText->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
	healthText->SetCharacterSize(15.0f);
	healthText->SetPos(glm::vec2(10.0f, 50.0f));

	healthBar = (RectangleShape*)AddElement(new RectangleShape(healthBarSize, shaderMgr->GetShader("SimpleShader")), 4);
	healthBar->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	healthBar->SetPos(healthBarPos);

	healthBarBack = (RectangleShape*)AddElement(new RectangleShape(healthBarSize, shaderMgr->GetShader("SimpleShader")), 3);
	healthBarBack->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 0.4f));
	healthBarBack->SetPos(healthBarPos);

	simulatedDamages = 0.0f;

	healthBarBackground = (RectangleShape*)AddElement(new RectangleShape(healthBarSize, shaderMgr->GetShader("SimpleShader")), 2);
	healthBarBackground->SetColor(glm::vec4(1.0f));
	healthBarBackground->SetPos(healthBarPos);

	healthBarFrame = (RectangleShape*)AddElement(
		new RectangleShape(healthBarSize + 2.0f*healthBarFrameOffset, shaderMgr->GetShader("SimpleShader")), 1);
	healthBarFrame->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
	healthBarFrame->SetPos(healthBarPos - healthBarFrameOffset);

	//Name
	pokeName = (Text*)AddElement(new Text(font, "", shaderMgr->GetShader("FontShader")));
	pokeName->SetCharacterSize(20.0f);
	pokeName->SetPos(glm::vec2(10.0, size.y - 25.0f));
	pokeName->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));

	RectangleShape* frame = (RectangleShape*)AddElement(new RectangleShape(size, shaderMgr->GetShader("SimpleShader")), -1);
	frame->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));

	if (poke != nullptr)
		pokeName->SetText(poke->GetName());
	else
		pokeName->SetText("None");
}
PokemonStatsBar::~PokemonStatsBar() {

}

void PokemonStatsBar::Update(double dt) {
	if (poke != nullptr) {
		healthText->SetText("PV : " + std::to_string((int)poke->GetHealth()) + "/"+ std::to_string((int)poke->GetMaxHealth()));
		healthBarBack->SetSize(glm::vec2((poke->GetHealth() / poke->GetMaxHealth()) * healthBarSize.x, healthBarSize.y));
		float x = std::max((poke->GetHealth() - simulatedDamages), 0.0f);
		healthBar->SetSize(glm::vec2((x / poke->GetMaxHealth()) * healthBarSize.x, healthBarSize.y));
	}
}

void PokemonStatsBar::Draw(glm::mat4& cameraMat) {
	if (poke == nullptr)
		return;
	Panel::Draw(cameraMat);
}

void PokemonStatsBar::SimulateDamages(float damages) {
	simulatedDamages = damages;
}

void PokemonStatsBar::SetPokemon(Pokemon* l_poke) {
	poke = l_poke;
	if (poke == nullptr)
		return;
	pokeName->SetText(poke->GetName());
}

glm::vec2 PokemonStatsBar::GetSize() { return size; }
Pokemon* PokemonStatsBar::GetPokemon() { return poke; }

/*---------------PokemonGUI---------------*/

PokemonGUI::PokemonGUI(Pokemon* l_poke, Font* l_font, SharedContext* l_context)
	: Panel(l_context->shaderManager), poke(l_poke), context(l_context)
{
	nbStepLeft = 0;
	ShaderManager* shadeMgr = context->shaderManager;
	statsBar = (PokemonStatsBar*)AddElement(new PokemonStatsBar(poke, l_font, shaderMgr));

	movesBar = (Panel*)AddElement(new Panel(shaderMgr));

	glm::vec2 moveBarPadding(30.0f, 15.0f);
	glm::vec2 moveBarFramePadding(20.0f, 20.0f);
	glm::vec2 moveBarsPos(statsBar->GetSize().x + moveBarPadding.x + moveBarFramePadding.x, -100.0f);
	for (int i = 0; i < 4; i++) {
		moveBars[i] = (PokemonMoveBar*)movesBar->AddElement(new PokemonMoveBar(l_font, shaderMgr, glm::vec2(0, 0)));
		glm::vec2 size = moveBars[i]->GetSize();
		if (i == 2)
			moveBars[i]->SetPos(moveBarsPos + glm::vec2(0, 0));
		if (i == 3)
			moveBars[i]->SetPos(moveBarsPos + glm::vec2(size.x + moveBarPadding.x, 0));
		if (i == 0)
			moveBars[i]->SetPos(moveBarsPos + glm::vec2(0, size.y + moveBarPadding.y));
		if (i == 1)
			moveBars[i]->SetPos(moveBarsPos + glm::vec2(size.x + moveBarPadding.x, size.y + moveBarPadding.y));
	}

	glm::vec2 moveBarsFrameSize(2 * moveBarFramePadding.x + moveBars[0]->GetSize().x * 2 + moveBarPadding.x,
		2 * moveBarFramePadding.y + moveBars[0]->GetSize().y * 2 + moveBarPadding.y);
	RectangleShape* moveBarsFrame = (RectangleShape*)movesBar->AddElement(
		new RectangleShape(moveBarsFrameSize, shaderMgr->GetShader("SimpleShader")), -1);
	moveBarsFrame->SetPos(moveBarsPos - moveBarFramePadding);
	moveBarsFrame->SetColor(glm::vec4(glm::vec3(1.0f), 0.5f));

	glm::vec boxSize = statsBar->GetSize();
	glm::vec2 aimedPokeStatsBarPadding(0.0f, 10.0f);
	glm::vec2 aimedPokeStatsBarPos(moveBarsPos.x + moveBarsFrameSize.x + moveBarFramePadding.x,0.0f);
	for (int i = 0; i < MAX_AIMED_POKE; i++) {
		aimedPokeStatsBar[i] = (PokemonStatsBar*) AddElement(new PokemonStatsBar(nullptr, l_font, shaderMgr));
		aimedPokeStatsBar[i]->SetPos(aimedPokeStatsBarPos);
		aimedPokeStatsBarPos.y -= boxSize.y + aimedPokeStatsBarPadding.y;
	}

	float charSize = 35.0f;
	glm::vec2 nbStepBarSize(220.0f, 50.0f);
	nbStepBar = (Panel*)AddElement(new Panel(shadeMgr));
	RectangleShape* nbStepFrame = (RectangleShape*)AddElement(new RectangleShape(shadeMgr->GetShader("SimpleShader")));
	nbStepFrame->SetPos(glm::vec2(boxSize.x + moveBarPadding.x, boxSize.y - nbStepBarSize.y));
	nbStepFrame->SetColor(glm::vec4(glm::vec3(1.0f), 0.5f));
	nbStepFrame->SetSize(nbStepBarSize);
	nbStepText = (Text*)AddElement(new Text(l_font, "Steps left", shadeMgr->GetShader("FontShader")));
	nbStepText->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
	nbStepText->SetCharacterSize(charSize);
	nbStepText->SetPos(glm::vec2(boxSize.x + moveBarPadding.x, boxSize.y - nbStepBarSize.y) + glm::vec2(5.0f, 10.0f));
}
PokemonGUI::~PokemonGUI() {

}

void PokemonGUI::Update(double dt) {
	statsBar->Update(dt);
	for (int i = 0; i < MAX_AIMED_POKE; i++) {
		aimedPokeStatsBar[i]->Update(dt);
	}
}

void PokemonGUI::Draw(glm::mat4& cameraMatrix) {
	if (poke == nullptr)
		return;
	Panel::Draw(cameraMatrix);
}

void PokemonGUI::SetPokemon(Pokemon* l_poke) {
	poke = l_poke;
	statsBar->SetPokemon(l_poke);
	if (poke == nullptr) {
		Reset();
		return;
	}
	for (int i = 0; i < 4; i++)
		moveBars[i]->SetPokemonMove(poke->GetMove(i));
}

Pokemon* PokemonGUI::GetPokemon() {
	return poke;
}

void PokemonGUI::Reset() {
	for (int i = 0; i < MAX_AIMED_POKE; i++) {
		aimedPokeStatsBar[i]->SetPokemon(nullptr);
	}
	SetSelectedMove(-1);
}

void PokemonGUI::SetSelectedMove(int i) {
	for (int j = 0; j < 4; j++)
		moveBars[j]->SetSelect(j == i);
	if (i < 0 || i > 3) {
		std::vector<Pokemon*> p;
		SetAimedPoke(p, nullptr);
	}
}

void PokemonGUI::SetNbStepsLeft(int l_nbStep) {
	if (nbStepLeft == l_nbStep)
		return;
	nbStepLeft = l_nbStep;
	std::string s = nbStepLeft < 2 ? "" : "s";
	nbStepText->SetText("Step" + s + " left : " + std::to_string(l_nbStep));
}

void PokemonGUI::SetAimedPoke(std::vector<Pokemon*>& aimedPoke, PokemonMove* move) {
	int i = 0;
	for (; i < MAX_AIMED_POKE && i < aimedPoke.size(); i++) {
		aimedPokeStatsBar[i]->SetPokemon(aimedPoke[i]);
		aimedPokeStatsBar[i]->SimulateDamages(context->gameSystem->ComputeDamages(poke, aimedPoke[i], move));
	}
	for (; i < MAX_AIMED_POKE; i++)
		aimedPokeStatsBar[i]->SetPokemon(nullptr);
}

int PokemonGUI::GetMoveClicked(glm::vec2 mousePos) {
	for (int i = 0; i < 4; i++) {
		if (moveBars[i]->IsClicked(mousePos))
			return i;
	}
	return -1;
}

/*---------------GUI---------------*/

GUI::GUI(SharedContext* l_context)
	: context(l_context),
	font(context->fontManager->RequireGetResource("Arial")),
	gameInfos(glm::vec2(175, 200), 14, glm::vec2(10,350), l_context->shaderManager),
	gameName(l_context->shaderManager),
	cursor(glm::vec2(100.0f, 100.0f), l_context->shaderManager->GetShader("SimpleShader")),
	hoverPokeBar(nullptr, font, context->shaderManager),
	selectedPokeGUI(nullptr, font, context)
{
	context->gui = this;

	//GameInfos
	gameInfos.SetPadding(glm::vec2(10.0f, 4.0f));
	
	//Cursor
	cursor.SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	cursor.SetSize(glm::vec2(10.0f, 10.0f));
	cursor.SetPos(glm::vec2(Constants::WIN_WIDTH / 2, Constants::WIN_HEIGHT / 2));
	cursor.SetOrigin(Location::Middle);

	//Game Name
	Text* text = (Text*)gameName.AddElement(
		new Text(font, "PokeTales", context->shaderManager->GetShader("FontShader")));
	text->SetPos(glm::vec2(5.0f, 5.0f));
	text->SetCharacterSize(40.0f);

	glm::vec2 gameNameSize = text->GetFloatRect().size + glm::vec2(10.0f, 10.0f);
	gameName.SetPos(glm::vec2(0, Constants::WIN_HEIGHT - gameNameSize.y));

	RectangleShape* frame = (RectangleShape*) gameName.AddElement(
		new RectangleShape(gameNameSize, context->shaderManager->GetShader("SimpleShader")));
	frame->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));

	//Hove pokemons stats bar
	hoverPokeBar.SetPos(glm::vec2(0.0f, 0.0f));

	//Selected pokemon GUI
	selectedPokeGUI.SetPos(glm::vec2(225.0f, Constants::WIN_HEIGHT - hoverPokeBar.GetSize().y));

}

GUI::~GUI() {
	context->fontManager->ReleaseResource("Arial");
}

void GUI::Update(double dt) {
	selectedPokeGUI.Update(dt);
	hoverPokeBar.Update(dt);
}

void GUI::Render() {
	glDisable(GL_DEPTH_TEST);

	context->win->DrawStatic(&cursor);
	context->win->DrawStatic(&gameInfos);
	context->win->DrawStatic(&gameName);

	if (selectedPokeGUI.GetPokemon() != hoverPokeBar.GetPokemon())
		context->win->DrawStatic(&hoverPokeBar);
	context->win->DrawStatic(&selectedPokeGUI);
	glEnable(GL_DEPTH_TEST);
}

void GUI::SetHoverPokemon(Pokemon* poke) {
	hoverPokeBar.SetPokemon(poke);
}
void GUI::UnsetHoverPokemon() {
	hoverPokeBar.SetPokemon(nullptr);
}

void GUI::SetSelectedPokemon(Pokemon* poke) {
	selectedPokeGUI.SetPokemon(poke);
}
void GUI::UnsetSelectedPokemon() {
	selectedPokeGUI.SetPokemon(nullptr);
}

TextField* GUI::GetGameInfosField() { return &gameInfos; }
PokemonGUI* GUI::GetSelectedPokemonGUI() { return &selectedPokeGUI; }