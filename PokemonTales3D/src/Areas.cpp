#include "Areas.h"

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

void SquareArea::SetSize(glm::ivec2 l_size) {
	size = l_size;
	SetIntRect();
	SetRealPosOffset();
	compute = true;
}

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
	if (pos == l_pos && !compute)
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

	compute = false;
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

/*------------------------MoveArea------------------------*/

bool MoveArea::IsIn(glm::ivec2 l_pos) {

	for (auto& itr : boxDistance) {
		if (itr.first->GetPos() == l_pos) {
			return itr.second >= 0;
		}
	}
	return false;
}

int MoveArea::Distance(Board* board, glm::ivec2 l_pos) {
	return boxDistance[board->GetBox(l_pos)];
}

void MoveArea::Update(Board* board, glm::ivec2 l_pos) {
	Clear();
	pos = l_pos;
	glm::ivec2 boardSize = board->GetSize();
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
		glm::ivec2 currPos = curr->GetPos();
		std::vector<glm::ivec2> neigh;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (i == 0 && j == 0)
					continue;
				neigh.emplace_back(currPos.x + i, currPos.y + j);
			}
		}
		for (glm::ivec2 n_pos : neigh) {
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
