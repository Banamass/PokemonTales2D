#include "Widgets.h"

/*------------------------------TextField------------------------------*/

TextField::TextField(sf::Vector2f l_backSize , unsigned int l_fontSize, sf::Vector2f  l_pos)
	: backSize(l_backSize), fontSize(l_fontSize), pos(l_pos){
	std::string fontName = "Resources\\arial.ttf";
	if (!font.openFromFile(fontName)) {
		std::cout << "Enable to open " << fontName << std::endl;
	}

	background.setSize(backSize);
	background.setFillColor(sf::Color(0, 0, 0, 150));
	background.setPosition(pos);
	background.setOutlineThickness(3.0f);
	background.setOutlineColor(sf::Color::Red);
	padding = sf::Vector2f(8, 5);
}

TextField::~TextField() {}

void TextField::AddMessage(const std::string& mess){
	if (mess == "")
		return;
	float maxTextWidth = backSize.x - padding.x*2;
	sf::Text textToAdd(font, "");
	textToAdd.setFillColor(sf::Color::Cyan);
	textToAdd.setCharacterSize(fontSize);
	int nbCharact = 0;
	while (textToAdd.getLocalBounds().size.x < maxTextWidth && nbCharact < mess.size()) {
		nbCharact++;
		textToAdd.setString(mess.substr(0, nbCharact));
	}
	if (nbCharact == mess.size()) {
		PushMessage(textToAdd);
		return;
	}

	int spaceInd = nbCharact-1;
	for (; spaceInd >= 0 && mess[spaceInd] != ' '; spaceInd--) {}

	textToAdd.setString(mess.substr(0, spaceInd));
	PushMessage(textToAdd);
	AddMessage(mess.substr(spaceInd+1, mess.size() - spaceInd));
}

void TextField::PushMessage(sf::Text& mess) {
	if ((messages.size()+1) * fontSize + padding.y * 2 > backSize.y) {
		messages.erase(messages.begin());
	}
	messages.push_back(mess);
}

void TextField::Render(Window* win) {
	win->Draw(background);
	sf::Vector2f posText({ pos.x + padding.x, pos.y + padding.y });
	for (int i = 0; i < messages.size(); i++) {
		messages[i].setPosition(posText);
		win->Draw(messages[i]);
		posText.y += fontSize;
	}
}