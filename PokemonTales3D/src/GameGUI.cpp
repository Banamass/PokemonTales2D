#include "GameGUI.h"

/*---------------PokemonTypeFrame---------------*/

PokemonTypeFrame::PokemonTypeFrame(PokeType l_pokeType, Font* l_font, ShaderManager* l_shaderManager)
	: Panel(l_shaderManager), size(50.0f, 20.0f), pokeType(l_pokeType)
{
	text = (Text*)AddElement(new Text(l_font, GetStringFromPokeType(pokeType), l_shaderManager->GetShader("FontShader")));
	frame = (RectangleShape*)AddElement(new RectangleShape(size, l_shaderManager->GetShader("SimpleShader")), -1);

	frame->SetColor(GetColorFromPokeType(pokeType));
	text->SetCharacterSize(14.0f);
	text->SetOrigin(Location::Middle);
	text->SetPos(size / 2.0f);
	text->SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
}
PokemonTypeFrame::~PokemonTypeFrame() {}

void PokemonTypeFrame::SetSize(glm::vec2 l_size) {
	size = l_size;
	frame->SetSize(size);
	text->SetPos(size / 2.0f);
}
void PokemonTypeFrame::SetCharacterSize(float charSize) {
	text->SetCharacterSize(charSize);
	text->SetOrigin(Location::Middle);
}
void PokemonTypeFrame::SetPokeType(PokeType l_pokeType) {
	pokeType = l_pokeType;
	frame->SetColor(GetColorFromPokeType(pokeType));
	text->SetText(GetStringFromPokeType(pokeType));
	text->SetOrigin(Location::Middle);
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
	selectedColor = glm::vec4(glm::vec3(0.6f), 1.0f);
	hoverColor = glm::vec4(glm::vec3(0.8f), 1.0f);

	ppText = (Text*)AddElement(new Text(l_font, "10/10", l_shaderMgr->GetShader("FontShader")));
	ppText->SetColor(glm::vec3(0.0f));
	ppText->SetCharacterSize(charSize);
	ppText->SetPos(glm::vec2(size.x - margin, margin));
	ppText->SetOrigin(Location::BottomRight);

	moveName = (Text*)AddElement(new Text(l_font, "Move", l_shaderMgr->GetShader("FontShader")));
	moveName->SetColor(glm::vec3(0.0f));
	moveName->SetCharacterSize(charSize);
	moveName->SetPos(glm::vec2(margin, size.y - charSize - margin));

	powerText = (Text*)AddElement(new Text(l_font, "Power : 50", l_shaderMgr->GetShader("FontShader")));
	powerText->SetColor(glm::vec3(0.0f));
	powerText->SetCharacterSize(charSize);
	powerText->SetPos(glm::vec2(margin, margin));

	rangeText = (Text*)AddElement(new Text(l_font, "0", l_shaderMgr->GetShader("FontShader")));
	rangeText->SetColor(glm::vec3(0.0f));
	rangeText->SetCharacterSize(charSize);
	rangeText->SetPos(glm::vec2(size.x / 2.0f, margin));
	rangeText->SetOrigin(Location::MiddleBottom);

	typeFramePadding = glm::vec2(2.0f, 2.0f);
	typeFrame = (PokemonTypeFrame*)AddElement(new PokemonTypeFrame(PokeType::NonePokeType, l_font, l_shaderMgr));
	typeFrame->SetSize(glm::vec2(55.0f, 17.0f));
	typeFrame->SetCharacterSize(13.0f);
	typeFrame->SetOrigin(Location::TopRight);
	typeFrame->SetPos(size - glm::vec2(margin));

	frame = (RectangleShape*)AddElement(new RectangleShape(size, l_shaderMgr->GetShader("SimpleShader")), -2);
	frame->SetColor(unselectedColor);
}
PokemonMoveBar::~PokemonMoveBar() {

}

void PokemonMoveBar::Draw(glm::mat4& cameraMatrix) {
	if (move != nullptr)
		Panel::Draw(cameraMatrix);
}

bool PokemonMoveBar::In(glm::vec2 mousePos) {
	if (move == nullptr)
		return false;
	FloatRect hitBox(GetRealPos(), size);
	return hitBox.Contains(mousePos);
}

void PokemonMoveBar::Hover() {
	if (!isSelected)
		frame->SetColor(hoverColor);
}

void PokemonMoveBar::UnHover() {
	frame->SetColor(isSelected ? selectedColor : unselectedColor);
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
	typeFrame->SetPokeType(move->data->type);
	typeFrame->SetOrigin(Location::TopRight);
	rangeText->SetText(std::to_string(move->data->range));
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
	healthText = (Text*)AddElement(new Text(font, "", shaderMgr->GetShader("FontShader")));
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
		new RectangleShape(healthBarSize + 2.0f * healthBarFrameOffset, shaderMgr->GetShader("SimpleShader")), 1);
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

	//Type
	std::pair<PokeType, PokeType> types;
	if (poke != nullptr) {
		types = poke->GetType();
	}
	else {
		types.first = PokeType::NonePokeType;
		types.second = PokeType::NonePokeType;
	}

	type1Frame = (PokemonTypeFrame*)AddElement(new PokemonTypeFrame(types.first, l_font, shaderMgr));
	type1Frame->SetSize(glm::vec2(65.0f, 20.0f));
	type1Frame->SetCharacterSize(15.0f);
	type1Frame->SetOrigin(Location::TopRight);
	type1Frame->SetPos(size - glm::vec2(10.0f, 10.0f));
	this->SetActivatedElement(type1Frame, types.first != PokeType::NonePokeType);

	type2Frame = (PokemonTypeFrame*)AddElement(new PokemonTypeFrame(types.second, l_font, shaderMgr));
	type2Frame->SetSize(glm::vec2(65.0f, 20.0f));
	type2Frame->SetCharacterSize(15.0f);
	type2Frame->SetOrigin(Location::TopRight);
	type2Frame->SetPos(size - glm::vec2(10.0f, 30.0f));
	this->SetActivatedElement(type2Frame, types.second != PokeType::NonePokeType);
}
PokemonStatsBar::~PokemonStatsBar() {

}

void PokemonStatsBar::Update(double dt) {
	if (poke != nullptr) {
		healthText->SetText("PV : " + std::to_string((int)poke->GetHealth()) + "/" + std::to_string((int)poke->GetMaxHealth()));
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
	auto types = poke->GetType();
	type1Frame->SetPokeType(types.first);
	type2Frame->SetPokeType(types.second);
	this->SetActivatedElement(type1Frame, types.first != PokeType::NonePokeType);
	this->SetActivatedElement(type2Frame, types.second != PokeType::NonePokeType);
}

glm::vec2 PokemonStatsBar::GetSize() { return size; }
Pokemon* PokemonStatsBar::GetPokemon() { return poke; }

/*---------------PokemonStatsPanel---------------*/

PokemonStatsPanel::PokemonStatsPanel(Pokemon* l_poke, Font* l_font, ShaderManager* l_shaderManager)
	: WindowTabs(l_shaderManager, l_font), poke(l_poke), nbMoves(0)
{
	SetCharSize(12.0f);

	statsPanel = AddTab("Stats");

	padding = 10.0f;

	glm::vec2 pos(0.0f);
	for (int i = 0; i < Constants::NB_STATS; i++) {
		statsName[i] = (Text*)statsPanel->AddElement(new Text(l_font, GetStringFromStat((Stat)i) + " : ", l_shaderManager->GetShader("FontShader")));
		statsName[i]->SetCharacterSize(20.0f);
		statsName[i]->SetColor(glm::vec3(0.0f));
		statsName[i]->SetPos(pos);
		stats[i] = (Text*)statsPanel->AddElement(new Text(l_font, "", l_shaderManager->GetShader("FontShader")));
		stats[i]->SetCharacterSize(20.0f);
		stats[i]->SetColor(glm::vec3(0.0f));
		stats[i]->SetPos(pos + glm::vec2(90.0f, 0.0f));
		pos.y -= 30.0f;
	}
	statsPanel->SetOrigin(Location::TopLeft);
	statsPanel->SetPos(glm::vec2(padding, size.y - padding));

	movesPanel = AddTab("Moves");
	glm::vec2 bmSize;
	bmSize.x = size.x / 2.0f - 2 * padding;
	bmSize.y = 22.0f;
	glm::vec2 bmPos;
	for (int i = 0; i < Constants::NB_MOVES_MAX_BY_POKE; i++) {
		bmPos.x = padding + (i % 2 == 0 ? 0 : size.x / 2.0f);
		bmPos.y = size.y - (i / 2) * (padding + bmSize.y) - padding;
		movesButton[i] = (Button*)movesPanel->AddElement(new Button(font, shaderMgr));
		movesButton[i]->SetText("button");
		movesButton[i]->SetCharacterSize(15.0f);
		movesButton[i]->SetTextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		movesButton[i]->SetSize(bmSize);
		movesButton[i]->SetOrigin(Location::TopLeft);
		movesButton[i]->SetPos(bmPos);
	}
	moveDetail = (TextBlock*)movesPanel->AddElement(new TextBlock(font, shaderMgr));

	descPanel = AddTab("Desc.");
	description = (TextBlock*)descPanel->AddElement(new TextBlock(font, shaderMgr));
	description->SetCharacterSize(20.0f);
	descPanel->SetPos(glm::vec2(padding, size.y - padding));

	SetPokemon(l_poke);
}
PokemonStatsPanel::~PokemonStatsPanel() {}

void PokemonStatsPanel::Draw(glm::mat4& cameraMat) {
	if (poke == nullptr)
		return;
	Panel::Draw(cameraMat);
}

void PokemonStatsPanel::Update(Window* win) {
	WindowTabs::Update(win);
	for (int i = 0; i < nbMoves; i++) {
		movesButton[i]->Update(win);
		if (movesButton[i]->GetClick()) {
			SetSelectedMove(i);
			break;
		}
	}
}

void PokemonStatsPanel::SetSelectedMove(int i) {
	if (selectedMove != -1) {
		movesButton[selectedMove]->SetActivated(true);
		movesButton[selectedMove]->SetFrameColor(glm::vec4(1.0f));
	}
	selectedMove = i;
	if (selectedMove != -1) {
		movesButton[selectedMove]->SetActivated(false);
		movesButton[selectedMove]->SetFrameColor(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
		std::stringstream ss;
		if (poke != nullptr) {
			PokemonStatus& status = poke->GetStatus();
			PokemonMove* m = status.movePool[selectedMove];
			ss << "Name  : " << m->data->name << "\n";
			ss << "Type  : " << GetStringFromPokeType(m->data->type) << "       ";
			ss << "Cat   : " << GetStringFromCatType(m->data->cat) << "\n";
			ss << "Power : " << m->data->power << "       ";
			ss << "Acc.  : " << m->data->acc << "\n";
			ss << "PP    : " << m->data->pp << "       ";
			ss << "Range : " << m->data->range << "\n";
			ss << "Zone  : " << m->data->hitZone.x << ", " << m->data->hitZone.y << "\n";
			ss << "Effect: " << m->data->effect << "\n";
			ss << "Prob  : " << m->data->prob;
		}
		moveDetail->SetText(ss.str());
		moveDetail->FitIn(size.x - 2 * padding);
		moveDetail->SetOrigin(Location::TopLeft);
	}
}

void PokemonStatsPanel::SetPokemon(Pokemon* l_poke) {
	poke = l_poke;
	for (int i = 0; i < Constants::NB_MOVES_MAX_BY_POKE; i++)
		movesPanel->SetActivatedElement(movesButton[i], false);

	if (poke == nullptr)
		return;

	PokemonStatus& status = poke->GetStatus();
	for (int i = 0; i < Constants::NB_STATS; i++) {
		stats[i]->SetText(std::to_string(status.GetStat((Stat)i)));
	}
	nbMoves = 0;
	for (int i = 0; i < Constants::NB_MOVES_MAX_BY_POKE; i++) {
		PokemonMove* m = status.movePool[i];
		if (m == nullptr)
			break;
		movesPanel->SetActivatedElement(movesButton[i], true);
		movesButton[i]->SetText(m->data->name);
		nbMoves++;
	}
	if (nbMoves > 0) {
		SetSelectedMove(0);
	}

	std::stringstream ss;
	ss << "Size : " << status.data->size.x << ", " << status.data->size.y;
	description->SetText(ss.str());
	description->SetOrigin(Location::TopLeft);
	statsPanel->SetOrigin(Location::TopLeft);
}

void PokemonStatsPanel::SetSize(glm::vec2 l_size) {
	WindowTabs::SetSize(l_size);
	statsPanel->SetPos(glm::vec2(padding, size.y - padding));
	glm::vec2 bmSize;
	bmSize.x = size.x / 2.0f - 2 * padding;
	bmSize.y = 22.0f;
	glm::vec2 bmPos;
	for (int i = 0; i < Constants::NB_MOVES_MAX_BY_POKE; i++) {
		bmPos.x = padding + (i % 2 == 0 ? 0 : size.x / 2.0f);
		bmPos.y = size.y - (i / 2) * (padding + bmSize.y) - padding;
		movesButton[i]->SetSize(bmSize);
		movesButton[i]->SetOrigin(Location::TopLeft);
		movesButton[i]->SetPos(bmPos);
	}
	moveDetail->SetPos(glm::vec2(padding, size.y - (Constants::NB_MOVES_MAX_BY_POKE) / 2 * (padding + bmSize.y) - 2 * padding));
	descPanel->SetPos(glm::vec2(padding, size.y - padding));
}