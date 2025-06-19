#include "APlayer.h"

/*
void SelectedBoxArea::UpdateCross(Board* board, sf::Vector2i centerCoord) {
	Add(board->GetBox(sf::Vector2i(centerCoord.x, centerCoord.y)));
	for (int i = 1; i < size; i++) {
		Add(board->GetBox(sf::Vector2i(centerCoord.x - i, centerCoord.y)));
		Add(board->GetBox(sf::Vector2i(centerCoord.x + i, centerCoord.y)));
		Add(board->GetBox(sf::Vector2i(centerCoord.x, centerCoord.y - i)));
		Add(board->GetBox(sf::Vector2i(centerCoord.x, centerCoord.y + i)));
	}
}
void SelectedBoxArea::UpdateDiamant(Board* board, sf::Vector2i centerCoord) {
	for (int i = (-size + 1); i <= (size - 1); i++) {
		for (int j = (-size + 1) + std::abs(i); j <= (size - 1) - std::abs(i); j++) {
			Add(board->GetBox(sf::Vector2i(centerCoord.x + i, centerCoord.y + j)));
		}
	}
}
*/
/*------------------------AbstractArea------------------------*/

void AbstractArea::Clear() {
	for (Box* box : boxes) {
		box->ResetColor();
	}
	boxes.clear();
}

void AbstractArea::Add(Box* box) {
	if (box == nullptr)
		return;
	boxes.push_back(box);
	box->SetColor(selectedColor);
}

/*------------------------SquareArea------------------------*/

sf::IntRect SquareArea::GetIntRect() {
	return intRect;
}

void SquareArea::setIntRect() {
	intRect = sf::IntRect({ pos.x - originOffset.x, pos.y - originOffset.y }, size);
}

bool SquareArea::IsIn(sf::Vector2i pos) {
	return false;
}

void SquareArea::Update(Board* board, sf::Vector2i l_pos) {
	Clear();
	sf::Vector2i boxPos;
	for (int i = 0; i < size.x; i++) {
		for (int j = 0; j < size.y; j++) {
			boxPos.x = l_pos.x + i - originOffset.x;
			boxPos.y = l_pos.y + j - originOffset.y;
			Add(board->GetBox(boxPos));
		}
	}
	pos = l_pos;
}

void SquareArea::setRealPosOffset() {
	if (origin == Location::Middle)
		originOffset = sf::Vector2i(size.x / 2, size.y / 2);
	else if (origin == Location::BottomLeft)
		originOffset = sf::Vector2i(0, size.y-1);
	else if (origin == Location::TopRight)
		originOffset = sf::Vector2i(size.x-1,0);
	else if (origin == Location::BottomRight)
		originOffset = sf::Vector2i(size.x-1, size.y-1);
}

/*------------------------MoveArea------------------------*/

bool MoveArea::IsIn(sf::Vector2i l_pos) {
	return sf::IntRect(pos - sf::Vector2i(range, range), sf::Vector2i(range, range) * 2).contains(l_pos);
}

void MoveArea::Update(Board* board, sf::Vector2i l_pos) {
	Clear();
	sf::Vector2i boxPos;
	sf::Vector2i size(range, range);
	for (int i = -size.x; i < size.x; i++) {
		for (int j = -size.y; j < size.y; j++) {
			boxPos.x = l_pos.x + i;
			boxPos.y = l_pos.y + j;
			Add(board->GetBox(boxPos));
		}
	}
	pos = l_pos;
}

/*------------------------APlayer------------------------*/

APlayer::APlayer(SharedContext* l_context) : context(l_context){
	
}
APlayer::~APlayer() {
	for (Pokemon* poke : pokemons) {
		delete poke;
	}
}