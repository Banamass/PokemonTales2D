#include "APlayer.h"
#include "Board.h"

/*------------------------AbstractArea------------------------*/

void AbstractArea::Clear() {
	boxes.clear();
}

void AbstractArea::Add(Box* box) {
	if (box == nullptr)
		return;
	if (std::find(boxes.begin(), boxes.end(), box) != boxes.end())
		return;
	boxes.push_back(box);
}

/*------------------------SquareArea------------------------*/

IntRect SquareArea::GetIntRect() {
	return intRect;
}

void SquareArea::SetIntRect() {
	intRect = IntRect({ pos.x - originOffset.x, pos.y - originOffset.y }, size);
}

bool SquareArea::IsIn(glm::ivec2 pos) {
	return false;
}

void SquareArea::Update(Board* board, glm::ivec2 l_pos) {
	if (pos == l_pos)
		return;
	Clear();
	glm::ivec2 boxPos;
	for (int i = 0; i < size.x; i++) {
		for (int j = 0; j < size.y; j++) {
			boxPos.x = l_pos.x + i - originOffset.x;
			boxPos.y = l_pos.y + j - originOffset.y;
			Add(board->GetBox(boxPos));
		}
	}
	pos = l_pos;
	SetIntRect();
}

void SquareArea::SetRealPosOffset() {
	if (origin == Location::Middle)
		originOffset = glm::ivec2(size.x / 2, size.y / 2);
	else if (origin == Location::BottomLeft)
		originOffset = glm::ivec2(0, size.y - 1);
	else if (origin == Location::TopRight)
		originOffset = glm::ivec2(size.x - 1, 0);
	else if (origin == Location::BottomRight)
		originOffset = glm::ivec2(size.x - 1, size.y - 1);
}