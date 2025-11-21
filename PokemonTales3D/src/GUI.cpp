#include "GUI.h"
#include "GameSystem.h"

/*---------------Clickable---------------*/

Clickable::Clickable()
	: press(false), click(false), hover(false), activated(true),
	pressColor(0.0f), hoverColor(0.0f)
{}

void Clickable::Update(Window* win) {
	if (!activated)
		return;
	SetClick(false);

	glm::vec2 mousePos = win->GetMousePos();

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

void Clickable::SetActivated(bool b) {
	activated = b;
	if (!b) {
		SetHover(false);
		SetClick(false);
		SetPress(false);
	}
}
bool Clickable::GetActivated() {
	return activated;
}

/*---------------Scrollable---------------*/

Scrollable::Scrollable() 
	: scrollXPos(0), scrollYPos(0), prevScrollXPos(0), prevScrollYPos(0) {}
Scrollable::~Scrollable(){}

void Scrollable::UpdateScrollPos(double xoffset, double yoffset) {
	prevScrollXPos = scrollXPos;
	prevScrollYPos = scrollYPos;

	scrollXPos += xoffset;
	scrollYPos += yoffset;
}

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
			delete elem.drawable;
}

void Panel::Draw(glm::mat4& cameraMatrix) {
	if (compute) {
		UpdateElementsOffset();
		compute = false;
	}

	for (auto& itr : elements) {
		for (auto& elem : itr.second) {
			if(elem.activated)
				elem.drawable->Draw(cameraMatrix);
		}
	}
}

DrawableStatic* Panel::AddElement(DrawableStatic* elem, int zindex) {
	auto itr = elements.find(zindex);
	if (itr == elements.end()) {
		itr = elements.emplace(zindex, std::vector<PanelElement>()).first;
	}
	itr->second.emplace_back(elem, true);
	elem->SetOffset(GetRealPos());
	return elem;
}

void Panel::DeleteElement(DrawableStatic* elem) {
	for (auto& layer : elements) {
		for (auto itr = layer.second.begin(); itr != layer.second.end(); itr++) {
			if (elem == itr->drawable) {
				layer.second.erase(itr);
				return;
			}
		}
	}
}

void Panel::UpdateElementsOffset() {
	for (auto& layer : elements) 
		for(auto& elem : layer.second)
			elem.drawable->SetOffset(GetRealPos());
}

FloatRect Panel::GetFloatRect() {
	if (compute) {
		UpdateElementsOffset();
		compute = false;
	}
	FloatRect rect(glm::vec2(Constants::WIN_WIDTH*10, Constants::WIN_HEIGHT*10), glm::vec2(-1000,-1000));
	for (auto& layer : elements) {
		for (PanelElement& pelem : layer.second) {
			if (!pelem.activated)
				continue;
			DrawableStatic* elem = pelem.drawable;
			FloatRect elemRect = elem->GetFloatRect();
			rect.pos.x = std::min(elemRect.pos.x, rect.pos.x);
			rect.pos.y = std::min(elemRect.pos.y, rect.pos.y);
			glm::vec2 tr = elemRect.pos + elemRect.size;
			rect.size.x = std::max(tr.x, rect.size.x);
			rect.size.y = std::max(tr.y, rect.size.y);
		}
	}
	rect.size -= rect.pos;
	return rect;
}

Panel::PanelElement* Panel::GetPanelElem(DrawableStatic* elem) {
	for (auto& layer : elements) {
		for (auto& pelem : layer.second) {
			if (pelem.drawable == elem)
				return &pelem;
		}
	}
	return nullptr;
}

void Panel::SetActivatedElement(DrawableStatic* elem, bool b) {
	PanelElement* pelem = GetPanelElem(elem);
	if (pelem == nullptr)
		return;
	pelem->activated = b;
}
bool Panel::GetActivatedElement(DrawableStatic* elem) {
	PanelElement* pelem = GetPanelElem(elem);
	if (pelem == nullptr)
		return false;
	return pelem->activated;
}

/*---------------Button---------------*/

Button::Button(Font* l_font, ShaderManager* l_shaderMgr, glm::ivec2 l_pos)
	: Panel(l_shaderMgr, l_pos), Clickable(), origin(Location::BottomLeft), size(0, 0), characterSize(10.0f)
{
	color = glm::vec4(1.0f);
	pressColor = color * glm::vec4(glm::vec3(0.8f), 1.0f);
	hoverColor = color * glm::vec4(glm::vec3(0.5f), 1.0f);

	frame = (RectangleShape*)AddElement(new RectangleShape(shaderMgr->GetShader("SimpleShader")), -1);
	frame->SetOrigin(Location::Middle);

	text = (Text*)AddElement(new Text(l_font, "", shaderMgr->GetShader("FontShader")));
	text->SetOrigin(Location::Middle);

	SetCharacterSize(characterSize);
	SetPos(l_pos);
	SetFrameColor(glm::vec4(1.0f));
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
	text->SetPos(l_size / 2.0f);
}
void Button::SetCharacterSize(float l_charSize) {
	characterSize = l_charSize;
	text->SetCharacterSize(characterSize);
	text->SetOrigin(Location::Middle);
}
void Button::SetText(std::string l_text) {
	text->SetText(l_text);
	text->SetOrigin(Location::Middle);
}
void Button::SetTextColor(glm::vec4 color) {
	text->SetColor(color);
}
void Button::SetFrameColor(glm::vec4 l_color) {
	color = l_color;
	pressColor = color * glm::vec4(glm::vec3(0.8f), 1.0f);
	hoverColor = color * glm::vec4(glm::vec3(0.5f), 1.0f);
	frame->SetColor(color);
}

std::string Button::GetText() { return text->GetText(); }

/*---------------EmptyButton---------------*/

EmptyButton::EmptyButton(ShaderManager* l_shaderMgr, glm::ivec2 l_pos)
	: Panel(l_shaderMgr, l_pos), Clickable(), origin(Location::BottomLeft), size(0, 0)
{
	frame = (RectangleShape*)AddElement(new RectangleShape(shaderMgr->GetShader("SimpleShader")), -1);
	frame->SetOrigin(Location::Middle);

	SetColor(glm::vec4(1.0f));

	SetPos(l_pos);
}
EmptyButton::~EmptyButton() {}

bool EmptyButton::In(glm::vec2 mousePos) {
	FloatRect hitbox(frame->GetRealPos(), frame->GetSize());
	return hitbox.Contains(mousePos);
}

void EmptyButton::Hover() {
	frame->SetColor(hoverColor);
}
void EmptyButton::UnHover() {
	frame->SetColor(color);
}

void EmptyButton::Press() {
	frame->SetColor(pressColor);
}
void EmptyButton::UnPress() {
	frame->SetColor(color);
}

void EmptyButton::SetPos(glm::vec2 l_pos) {
	Panel::SetPos(l_pos - LocationToPosition(size, origin));
}
void EmptyButton::SetOrigin(Location l_origin) {
	origin = l_origin;
	SetPos(pos);
}
void EmptyButton::SetSize(glm::vec2 l_size) {
	size = l_size;
	frame->SetSize(l_size);
}
void EmptyButton::SetColor(glm::vec4 l_color) {
	color = l_color;
	pressColor = color * glm::vec4(glm::vec3(0.8f), 1.0f);
	hoverColor = color * glm::vec4(glm::vec3(0.5f), 1.0f);
	frame->SetColor(color);
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

void TextField::Reset() {
	for (auto itr = lines.begin(); itr != lines.end(); itr++) {
		DeleteElement(*(itr));
	}
	lines.clear();
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

/*---------------SelectBox---------------*/

void SelectBox::Setup(const std::vector<std::string>& fields, std::string l_defaultField) {
	activButtonsInt.first = boxes.begin();
	activButtonsInt.first = boxes.end();
	
	defaultField = l_defaultField;
	
	textColor = glm::vec4(glm::vec3(0.0f), 1.0f);
	frameColor = glm::vec4(glm::vec3(0.7f), 1.0f);
	charSize = bSize.y * 0.3f;

	mainBox = (Button*)AddElement(new Button(font, shaderMgr, glm::vec2(0, 0)));
	mainBox->SetSize(bSize);
	mainBox->SetTextColor(textColor);
	mainBox->SetCharacterSize(charSize);
	mainBox->SetText(defaultField);

	reduc = 0.9f;
	panelPadding = glm::vec2(5.0f, 10.0f);

	boxesPanel = (Panel*)AddElement(new Panel(shaderMgr));
	boxesPanel->SetPos(glm::vec2(0, bSize.y + panelPadding.y));

	panelFrame = (RectangleShape*)boxesPanel->AddElement(new RectangleShape(shaderMgr->GetShader("SimpleShader")), -1);
	panelFrame->SetColor(frameColor);
	panelFrame->SetPos(glm::vec2(0.0f, -panelPadding.y));
	panelFrame->SetSize(glm::vec2(
		panelPadding.x * 2 + bSize.x * reduc,
		(bSize.y * reduc + panelPadding.y) * listSize + panelPadding.y
	));

	for (auto& field : fields)
		AddField(field);

	SetActButtonsInt(0, listSize);

	SetInSelection(false);
	selectedBox = nullptr;
}
SelectBox::SelectBox(Font* l_font, ShaderManager* l_shaderMgr
	, glm::vec2 size, int l_listSize, std::string l_defaultField)
	: Panel(l_shaderMgr), Clickable(), Scrollable()
	,font(l_font), shaderMgr(l_shaderMgr), bSize(size), listSize(l_listSize)
{
	std::vector<std::string> fields;
	Setup(fields, l_defaultField);
}
SelectBox::SelectBox(Font* l_font, ShaderManager* l_shaderMgr, const std::vector<std::string>& fields
	, glm::vec2 size, int l_listSize, std::string l_defaultField)
	: Panel(l_shaderMgr), Clickable(), Scrollable()
	, font(l_font), shaderMgr(l_shaderMgr), bSize(size), listSize(l_listSize)
{
	Setup(fields, l_defaultField);
}
SelectBox::~SelectBox(){}

void SelectBox::Update(Window* win) {
	Clickable::Update(win);
	mainBox->Update(win);
	if(isInSelection) {
		for (auto& b : boxes)
			b->Update(win);
		if (!GetHover()) {
			SetInSelection(false);
		}
		else {
			for (auto& b : boxes) {
				if (b->GetClick() && b != selectedBox) {
					SetSelectedBox(b);
					SetInSelection(false);
					break;
				}
			}
		}
	}
	if (mainBox->GetClick()) {
		SetInSelection(true);
	}
}
void SelectBox::Draw(glm::mat4& cameraMatrix) {
	Panel::Draw(cameraMatrix);
}
void SelectBox::Scroll(double xoffset, double yoffset) {
	UpdateScrollPos(xoffset, yoffset);
	
	ShiftActButtonsInt(yoffset > 0 ? Direction::Up : Direction::Down);
}

void SelectBox::SetSelectedBox(Button* box) {
	if (selectedBox != nullptr) {
		selectedBox->SetTextColor(textColor);
	}
	else {
		mainBox->SetTextColor(textColor);
	}
	selectedBox = box;
	if (selectedBox != nullptr) {
		selectedBox->SetTextColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		mainBox->SetText(selectedBox->GetText());
	}
	else {
		mainBox->SetText(defaultField);
	}
}

void SelectBox::AddField(const std::string& field) {
	glm::vec2 bpos = glm::vec2(0.0f, 0.0f);

	boxes.push_back(((Button*)boxesPanel->AddElement(
		new Button(font, shaderMgr, bpos)
	)));
	Button* newB = *(boxes.end()-1);
	newB->SetSize(bSize * reduc);
	newB->SetTextColor(textColor);
	newB->SetCharacterSize(charSize * reduc);
	newB->SetText(field);
	boxesPanel->SetActivatedElement(newB, false);
}

bool SelectBox::In(glm::vec2 mousePos) {
	FloatRect hitBox = FloatRect(panelFrame->GetRealPos(), panelFrame->GetSize());
	return hitBox.Contains(mousePos) || mainBox->In(mousePos);
}

void SelectBox::SetInSelection(bool b) {
	isInSelection = b;
	for (auto itr = activButtonsInt.first; itr != activButtonsInt.second; itr++)
		boxesPanel->SetActivatedElement(*itr, b);
	boxesPanel->SetActivatedElement(panelFrame, b);
}

void SelectBox::UpdateButtonsScroll() {
	glm::vec2 bpos = glm::vec2(panelPadding.x, 0.0f);

	auto itr = boxes.begin();
	for (; itr != activButtonsInt.first; itr++) {
		boxesPanel->SetActivatedElement(*itr, false);
	}
	for (; itr != activButtonsInt.second; itr++) {
		(*itr)->SetPos(bpos);
		boxesPanel->SetActivatedElement(*itr, isInSelection);
		bpos.y += bSize.y * reduc + panelPadding.y;
	}
	for (; itr != boxes.end(); itr++) {
		boxesPanel->SetActivatedElement(*itr, false);
	}
}

bool SelectBox::SetActButtonsInt(int first, int last) {
	if (first < 0)
		return false;
	int i = 0;
	activButtonsInt.first = boxes.begin();
	while (i != first) {
		if (activButtonsInt.first == boxes.end())
			return false;
		activButtonsInt.first++;
		i++;
	}
	activButtonsInt.second = activButtonsInt.first;
	if (last < first)
		return false;
	while (i != last) {
		if (activButtonsInt.second == boxes.end())
			return false;
		activButtonsInt.second++;
		i++;
	}
	UpdateButtonsScroll();
	return true;
}
bool SelectBox::ShiftActButtonsInt(Direction dir) {
	if (dir == Direction::Up) {
		if (activButtonsInt.second == boxes.end())
			return false;
		activButtonsInt.first++;
		activButtonsInt.second++;
	}
	else if (dir == Direction::Down) {
		if (activButtonsInt.first == boxes.begin())
			return false;
		activButtonsInt.first--;
		activButtonsInt.second--;
	}
	else
		return false;
	UpdateButtonsScroll();
	return true;
}

void SelectBox::ResetSelectedField() {
	SetSelectedBox(nullptr);
}
void SelectBox::SetSelectedField(const std::string& field) {
	for (auto b : boxes) {
		if (field == b->GetText()) {
			SetSelectedBox(b);
		}
	}
}
std::string SelectBox::GetSelectedField() {
	return mainBox->GetText();
}
bool SelectBox::GetIsInSelection() { return isInSelection; }

/*---------------ColorSelection---------------*/

ColorSelection::ColorButton::ColorButton(ShaderManager* l_shaderMgr, glm::vec4 l_color, glm::vec2 size)
	: Panel(l_shaderMgr), color(l_color) {
	button = (EmptyButton*)AddElement(new EmptyButton(l_shaderMgr, glm::ivec2(0, 0)));
	button->SetColor(glm::vec4(1.0f));
	colorRect = (RectangleShape*)AddElement(new RectangleShape(l_shaderMgr->GetShader("SimpleShader")));
	colorRect->SetColor(l_color);
	SetSize(size);
}
ColorSelection::ColorButton::~ColorButton() {}

bool ColorSelection::ColorButton::GetClick() {
	return button->GetClick();
}
glm::vec4 ColorSelection::ColorButton::GetColor() {
	return color;
}

void ColorSelection::ColorButton::Update(Window* win) {
	button->Update(win);
}

void ColorSelection::ColorButton::SetSize(glm::vec2 size) {
	button->SetSize(size);
	colorRect->SetSize(size * 0.6f);
	colorRect->SetPos(size * 0.2f);
}

void ColorSelection::ColorButton::Desactivate(glm::vec4 desColor) {
	button->SetActivated(false);
	button->SetColor(desColor);
}
void ColorSelection::ColorButton::Activate() {
	button->SetActivated(true);
	button->SetColor(glm::vec4(1.0f));
}

ColorSelection::ColorSelection(ShaderManager* l_shaderMgr, Orientation l_orientation)
	: Panel(l_shaderMgr), Notifier(),
	orientation(l_orientation), shaderMgr(l_shaderMgr), selectIndex(-1) {
	SetSize(glm::vec2(20.0f, 20.0f));
}
ColorSelection::~ColorSelection() {}

void ColorSelection::SwitchSelectedColor(int newSelectIndex) {
	if (selectIndex != -1)
		buttons[selectIndex]->Activate();
	selectIndex = newSelectIndex;
	buttons[selectIndex]->Desactivate(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
	NotifyAll("Color");
}

void ColorSelection::Update(Window* win) {
	int i = 0;
	for (auto& b : buttons) {
		b->Update(win);
		if (b->GetClick()) {
			SwitchSelectedColor(i);
			break;
		}
		i++;
	}
}
void ColorSelection::Draw(glm::mat4& cameraMatrix) {
	Panel::Draw(cameraMatrix);
}

void ColorSelection::AddColor(glm::vec3 color, bool set) {
	AddColor(glm::vec4(color, 1.0f), set);
}
void ColorSelection::AddColor(glm::vec4 color, bool set) {
	glm::vec2 bpos = glm::vec2(buttons.size() * size.x, 0.0f);
	ColorButton* newB = (ColorButton*)AddElement(new ColorButton(shaderMgr, color, size));
	buttons.push_back(newB);
	newB->SetPos(bpos);
	if (set)
		SwitchSelectedColor(buttons.size()-1);
}

void ColorSelection::SetDesactivatedColors(std::vector<glm::vec4>& colors) {
	for (auto& b : desactivatedButtons)
		b->Activate();
	desactivatedButtons.clear();
	for (auto& c : colors) {
		ColorButton* b = GetColorButton(c);
		b->Desactivate(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		desactivatedButtons.push_back(b);
	}
}

ColorSelection::ColorButton* ColorSelection::GetColorButton(glm::vec4 color) {
	for (auto& b : buttons) {
		if (b->GetColor() == color)
			return b;
	}
}

void ColorSelection::SetSelectedColor(glm::vec4 color) {
	int i = 0;
	for (auto& b : buttons) {
		if (b->GetColor() == color)
			SwitchSelectedColor(i);
		i++;
	}
}

void ColorSelection::SetSize(glm::vec2 l_size) {
	size = l_size;

	glm::vec2 bpos = glm::vec2(0.0f, 0.0f);
	glm::vec2 padd = glm::vec2(size.x, 0.0f);
	for (auto& b : buttons) {
		b->SetSize(size);
		b->SetPos(bpos);
		bpos += padd;
	}
}

glm::vec4 ColorSelection::GetSelectedColor() {
	if (selectIndex == -1)
		return glm::vec4(0.0f);
	return buttons[selectIndex]->GetColor();
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

	ppText = (Text*)AddElement( new Text(l_font, "10/10", l_shaderMgr->GetShader("FontShader")));
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

	typeText = (Text*)AddElement(new Text(l_font, "Type1", l_shaderMgr->GetShader("FontShader")));
	typeText->SetColor(glm::vec3(0.0f));
	typeText->SetCharacterSize(charSize);
	typeText->SetPos(glm::vec2(size.x - 50, size.y - charSize - margin));
	
	rangeText = (Text*)AddElement(new Text(l_font, "0", l_shaderMgr->GetShader("FontShader")));
	rangeText->SetColor(glm::vec3(0.0f));
	rangeText->SetCharacterSize(charSize);
	rangeText->SetPos(glm::vec2(size.x / 2.0f, margin));
	rangeText->SetOrigin(Location::MiddleBottom);

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
	typeText->SetText(GetStringFromPokeType(move->data->type));
	typeFrame->SetColor(GetColorFromPokeType(move->data->type));
	typeFrame->SetSize(glm::vec2(typeText->GetFloatRect().size.x, charSize) + 2.0f * typeFramePadding);
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