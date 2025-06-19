#include "Pokemon.h"

Pokemon::Pokemon(int size, sf::Color color, sf::Vector2i l_size) :size(l_size) {
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
}
Pokemon::~Pokemon() {

}

void Pokemon::Render(Window* win, sf::Vector2f pos) {
	sprite.setPosition(pos);
	frame.setPosition(pos);
	win->Draw(frame);
	win->Draw(sprite);
}

sf::Vector2i Pokemon::GetSize() { return size; }
sf::RectangleShape Pokemon::GetFrame () { return frame; }