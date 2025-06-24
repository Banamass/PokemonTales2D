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
	setIntRect();
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

	for (auto& itr : boxDistance) {
		if (itr.first->GetBoardPos() == l_pos) {
			return itr.second >= 0;
		}
	}
	return false;
}

int MoveArea::Distance(Board* board, sf::Vector2i l_pos) {
	return boxDistance[board->GetBox(l_pos)];
}

void MoveArea::Update(Board* board, sf::Vector2i l_pos) {
	Clear();
	pos = l_pos;
	sf::Vector2i boardSize = board->GetSize();
	for (int i = 0; i < boardSize.x; i++) {
		for (int j = 0; j < boardSize.y; j++) {
			boxDistance.emplace(board->GetBox({ i, j }), -2); //-2 corresponds to an unvisited box
		}
	}

	Box* originBox = board->GetBox(pos);
	boxDistance[originBox] = 0;
	std::queue<Box*> toExplore;
	toExplore.push(originBox);
	Add(originBox);
	while (!toExplore.empty()) {
		Box* curr = toExplore.front();
		toExplore.pop();
		int currRange = boxDistance[curr];
		if (currRange == range)
			continue;
		sf::Vector2i currPos = curr->GetBoardPos();
		std::vector<sf::Vector2i> neigh;
		for (int i = -1; i < 2; i++){
			for (int j = -1; j < 2; j++) {
				if (i == 0 && j == 0)
					continue;
				neigh.emplace_back(currPos.x + i, currPos.y + j);
			}
		}
		for (sf::Vector2i n_pos : neigh) {
			Box* n = board->GetBox(n_pos);
			if (n == nullptr || boxDistance[n] != -2)
				continue;
			if (!board->CheckMove(movingPokemon, n_pos)) {
				boxDistance[n] = -1;
				continue;
			}
			boxDistance[n] = currRange + 1;
			toExplore.push(n);
			Add(n);
		}
	}
}

void MoveArea::Clear() {
	AbstractArea::Clear();
	boxDistance.clear();
}

/*------------------------APlayer------------------------*/

APlayer::APlayer(SharedContext* l_context) 
	: context(l_context), isPlaying(false){
	
}
APlayer::~APlayer() {
	for (Pokemon* poke : pokemons) {
		delete poke;
	}
}

void APlayer::PlayTurn() {
	isPlaying = true;
}

bool APlayer::Playing() { return isPlaying; }