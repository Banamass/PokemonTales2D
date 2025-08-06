#include "GUI.h"
#include "GameSystem.h"

/*---------------Clickable---------------*/

Clickable::Clickable() 
	: press(false), click(false), hover(false)
{}

void Clickable::Update(Window* win) {
	SetClick(false);

	glm::vec2 mousePos = win->GetMousePos();
	mousePos = glm::vec2(mousePos.x, Constants::WIN_HEIGHT - mousePos.y);

	bool in = In(mousePos);
	
	SetHover(in);

	if (glfwGetMouseButton(win->GetGLFWwindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && in) {
		SetPress(true);
	}

	if (glfwGetMouseButton(win->GetGLFWwindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		if (press && in)
			SetClick(true);
		SetPress(false);
	}
}

void Clickable::SetPress(bool b) {
	if (b == press)
		return;
	if (b && !press)
		Press();
	else if (!b && press)
		UnPress();
	press = b;
}
void Clickable::SetHover(bool b) {
	if (b == hover)
		return;
	if (b && !hover)
		Hover();
	else if (!b && hover)
		UnHover();
	hover = b;
}
void Clickable::SetClick(bool b) {
	if (b == click)
		return;
	if (b && !click)
		Click();
	else if (!b && click)
		UnClick();
	click = b;
}

bool Clickable::GetClick() { return click; }
bool Clickable::GetPress() { return press; }
bool Clickable::GetHover() { return hover; }

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

/*---------------Panel---------------*/

Button::Button(Font* l_font, ShaderManager* l_shaderMgr, glm::ivec2 l_pos)
	: Panel(l_shaderMgr, l_pos), Clickable(), origin(Location::BottomLeft), size(0, 0), characterSize(10.0f)
{
	color = glm::vec4(1.0f);
	pressColor = color;
	hoverColor = color;

	frame = (RectangleShape*)AddElement(new RectangleShape(shaderMgr->GetShader("SimpleShader")), -1);

	text = (Text*)AddElement(new Text(l_font, "", shaderMgr->GetShader("FontShader")));

	SetCharacterSize(characterSize);
	SetPos(l_pos);
}
Button::~Button() {

}

bool Button::In(glm::vec2 mousePos) {
	FloatRect hitbox(frame->GetRealPos(), frame->GetSize());
	return hitbox.Contains(mousePos);
}

void Button::Hover() {
	frame->SetColor(hoverColor);
}
void Button::UnHover() {
	frame->SetColor(color);
}

void Button::Press() {
	frame->SetColor(pressColor);
}
void Button::UnPress() {
	frame->SetColor(color);
}

void Button::SetPos(glm::vec2 l_pos) {
	Panel::SetPos(l_pos - LocationToPosition(size, origin));
}
void Button::SetOrigin(Location l_origin) {
	origin = l_origin;
	SetPos(pos);
}
void Button::SetSize(glm::vec2 l_size) {
	size = l_size;
	frame->SetSize(l_size);
	text->SetPos(size / 2.0f - (text->GetFloatRect().size / 2.0f));
}
void Button::SetCharacterSize(float l_charSize) {
	characterSize = l_charSize;
	text->SetCharacterSize(characterSize);
	text->SetPos(size / 2.0f - (text->GetFloatRect().size / 2.0f));
}
void Button::SetText(std::string l_text) {
	text->SetText(l_text);
	text->SetPos(size / 2.0f - (text->GetFloatRect().size / 2.0f));
}
void Button::SetTextColor(glm::vec4 color) {
	text->SetColor(color);
}
void Button::SetFrameColor(glm::vec4 color) {
	frame->SetColor(color);
}
void Button::SetFrameHoverColor(glm::vec4 color) {
	hoverColor = color;
}
void Button::SetFramePressColor(glm::vec4 color) {
	pressColor = color;
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

 bool PokemonMoveBar::In(glm::vec2 mousePos) {
	 if (move == nullptr)
		 return false;
	 FloatRect hitBox(GetRealPos(), size);
	 return hitBox.Contains(mousePos);
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