#include "Board.h"

/*------------------------Box------------------------*/

Box::Box(sf::Vector2f l_pos, float l_size, sf::Color color): pos(l_pos), size(l_size), defaultColor(color){

	sprite.setPosition(pos);
	sprite.setSize(sf::Vector2f(size, size));
	sprite.setFillColor(defaultColor);
	sprite.setOutlineThickness(2.0f);
	sprite.setOutlineColor(sf::Color::Blue);
}
Box::~Box(){}

void Box::Render(Window* win){
	win->Draw(sprite);
}

void Box::SetColor(sf::Color color) {
	sprite.setFillColor(color);
}
void Box::ResetColor() {
	sprite.setFillColor(defaultColor);
}

/*------------------------SelectedBoxArea------------------------*/

void SelectedBoxArea::Update(Board* board, sf::Vector2i centerCoord) {
	if (center == centerCoord)
		return;
	Clear();
	if (shape == Shape::Square) {
		UpdateSquare(board, centerCoord);
	}
	else if (shape == Shape::Cross) {
		UpdateCross(board, centerCoord);
	}
	else if (shape == Shape::Diamant) {
		UpdateDiamant(board, centerCoord);
	}
}

void SelectedBoxArea::Clear() {
	for (Box* box : boxes) {
		box->ResetColor();
	}
	boxes.clear();
}

void SelectedBoxArea::SetSize(int l_size) { size = l_size; }
void SelectedBoxArea::SetShape(Shape l_shape) { shape = l_shape; }
void SelectedBoxArea::SetSelectedColor(sf::Color color) { selectedColor = color; }

void SelectedBoxArea::UpdateSquare(Board* board, sf::Vector2i centerCoord) {
	for (int i = centerCoord.x - size + 1; i <= centerCoord.x + size - 1; i++) {
		for (int j = centerCoord.y - size + 1; j <= centerCoord.y + size - 1; j++) {
			Add(board->GetBox(sf::Vector2i(i, j)));
		}
	}
}
void SelectedBoxArea::UpdateCross(Board* board, sf::Vector2i centerCoord) {
	Add(board->GetBox(sf::Vector2i(centerCoord.x, centerCoord.y)));
	for (int i = 1; i < size; i++) {
		Add(board->GetBox(sf::Vector2i(centerCoord.x-i, centerCoord.y)));
		Add(board->GetBox(sf::Vector2i(centerCoord.x+i, centerCoord.y)));
		Add(board->GetBox(sf::Vector2i(centerCoord.x, centerCoord.y-i)));
		Add(board->GetBox(sf::Vector2i(centerCoord.x, centerCoord.y+i)));
	}
}
void SelectedBoxArea::UpdateDiamant(Board* board, sf::Vector2i centerCoord) {
	for (int i = (-size + 1); i <= (size - 1); i++) {
		for (int j = (-size + 1) + std::abs(i); j <= (size - 1)-std::abs(i); j++) {
			Add(board->GetBox(sf::Vector2i(centerCoord.x +i, centerCoord.y + j)));
		}
	}
}

// Add box to boxes if it's not null, otherwise do nothing
void SelectedBoxArea::Add(Box* box) {
	if (box == nullptr)
		return;
	boxes.push_back(box);
	box->SetColor(selectedColor);
}

/*------------------------Board------------------------*/

Board::Board(SharedContext* l_context): context(l_context){
	boxSize = 50;
	size = { 22,13 };
	CreateBoxes();
	cursor.SetSize(5);
	cursor.SetShape(SelectedBoxArea::Shape::Diamant);
	cursor.SetSelectedColor(sf::Color::Blue);

	SetCallbacks();
}
Board::~Board(){}

void Board::Update(){
	sf::FloatRect viewSpace = context->window->GetViewSpace();
	viewSpace.position.x = size.x * boxSize / 2 - viewSpace.size.x / 2;
	viewSpace.position.y = size.y * boxSize / 2 - viewSpace.size.y / 2;
	context->window->GetRenderWindow()->setView(sf::View(viewSpace));
}

void Board::Render(){
	for (int i = 0; i < size.x; i++) {
		for (int j = 0; j < size.y; j++) {
			boxes[i][j].Render(context->window);
		}
	}
}

Box* Board::GetBox(sf::Vector2i boxCoord) {
	return isBoxInBoard(boxCoord) ? &boxes[boxCoord.x][boxCoord.y] : nullptr;
}

void Board::SetCallbacks() {
	Binding* b = new Binding("MouseMove");
	b->BindEvent(new WindowEvent(WindowEvent::EType::MouseMove));
	context->eventManager->AddBinding(b);

	context->eventManager->AddCallback("MouseMove", &Board::UpdateCursor, this);
}

void Board::CreateBoxes() {
	for (int i = 0; i < size.x; i++) {
		boxes.push_back({});
		for (int j = 0; j < size.y; j++) {
			boxes[i].emplace_back(sf::Vector2f(i * boxSize, j * boxSize), boxSize);
		}
	}
}

bool Board::isBoxInBoard(sf::Vector2i boxCoord) {
	return boxCoord.x >= 0 && boxCoord.x < size.x
		&& boxCoord.y >= 0 && boxCoord.y < size.y;
}

void Board::UpdateCursor() {
	sf::Vector2i pix_mousepos = context->window->GetMousePos();
	sf::Vector2f world_mousePos = context->window->GetRenderWindow()->mapPixelToCoords(pix_mousepos);

	sf::Vector2i hoverBox((int)std::floor(world_mousePos.x / boxSize), (int)std::floor(world_mousePos.y / boxSize));
	cursor.Update(this, hoverBox);	
}
