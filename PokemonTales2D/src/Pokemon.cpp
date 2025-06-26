#include "Pokemon.h"

Pokemon::Pokemon(const std::string& l_name,int size, sf::Color color, sf::Vector2i l_size, APlayer* l_trainer)
	: name(l_name), size(l_size), trainer(l_trainer)
	, healthBar(&health, 100, WidgetsAttributs({100, 10}, sf::Color::Red, {-50, -Constants::BOX_SIZE}, 3.0f, sf::Color(255,255,255, 200), sf::Color::Black)) {
	sprite.setFillColor(color);
	sprite.setRadius(size);
	sprite.setOrigin({ (float)size, (float)size });

	float oulineThickness = Constants::GAP_SIZE;
	frame.setSize(sf::Vector2f({ (float)(l_size.x * Constants::BOX_SIZE) - oulineThickness
		, (float)(l_size.y * Constants::BOX_SIZE) - oulineThickness }));
	frame.setOutlineThickness(oulineThickness);
	frame.setFillColor(sf::Color(0, 0, 0, 0));
	frame.setOutlineColor(color);
	frame.setOrigin({ (float)Constants::BOX_SIZE / 2 - oulineThickness
		, (float)Constants::BOX_SIZE / 2 - oulineThickness });

	health = 100;
	moveRange = 0;
}
Pokemon::~Pokemon() {

}

void Pokemon::Render(Window* win, sf::Vector2f pos) {
	sprite.setPosition(pos);
	frame.setPosition(pos);
	win->Draw(frame);
	win->Draw(sprite);
	healthBar.Render(win, pos);
}

void Pokemon::TakeDamages(float l_damages) {
	health -= l_damages;
}

sf::Vector2i Pokemon::GetSize() { return size; }
sf::RectangleShape Pokemon::GetFrame () { return frame; }
int Pokemon::GetMoveRange() { return moveRange; }
void Pokemon::SetMoveRange(int l_range) { moveRange = l_range; }
APlayer* Pokemon::GetTrainer() { return trainer; }
std::string Pokemon::GetName() { return name; }
bool Pokemon::IsKO() { return health <= 0; }