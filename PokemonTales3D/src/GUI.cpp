#include "GUI.h"

/*---------------TextField---------------*/

TextField::TextField(glm::vec2 l_backSize, unsigned int l_fontSize, glm::vec2 l_pos, ShaderManager* l_shaderMgr)
	: backSize(l_backSize), fontSize(l_fontSize), pos(l_pos),
	shaderMgr(l_shaderMgr), textBox(l_shaderMgr->GetShader("SimpleShader"))
{
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

/*---------------GUI---------------*/

GUI::GUI(SharedContext* l_context)
	: context(l_context), cursorModel("Resources\\Box\\box.obj"),
	gameInfos(glm::vec2(175, 200), 14, glm::vec2(10,10), l_context->shaderManager),
	rect(glm::vec2(100.0f, 100.0f), l_context->shaderManager->GetShader("SimpleShader"))
{
	context->gui = this;

	gameInfos.SetPadding(glm::vec2(10.0f, 4.0f));
	
	rect.SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	rect.SetSize(glm::vec2(10.0f, 10.0f));
	rect.SetPos(glm::vec2(Constants::WIN_WIDTH / 2, Constants::WIN_HEIGHT / 2));
	rect.SetOrigin(Location::Middle);
}

void GUI::Render() {
	glDisable(GL_DEPTH_TEST);
	
	context->win->DrawStatic(&rect);
	gameInfos.Render(context->win);

	glEnable(GL_DEPTH_TEST);
}