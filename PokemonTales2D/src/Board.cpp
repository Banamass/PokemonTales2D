#include "Board.h"

/*------------------------Box------------------------*/

Box::Box(sf::Vector2f l_pos, sf::Vector2i l_boardPos, float l_size, sf::Color color)
	: pos(l_pos), size(l_size), defaultColor(color), boardPos(l_boardPos){
	float oulineThickness = Constants::GAP_SIZE;

	sprite.setPosition({ pos.x + oulineThickness, pos.y + oulineThickness });
	sprite.setSize(sf::Vector2f(size-oulineThickness, size-oulineThickness));
	sprite.setFillColor(defaultColor);
	sprite.setOutlineThickness(oulineThickness);
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

sf::Vector2f Box::GetPos() {
	return pos;
}

sf::Vector2i Box::GetBoardPos() {
	return boardPos;
}

/*------------------------Board------------------------*/

Board::Board(SharedContext* l_context): context(l_context){
	boxSize = Constants::BOX_SIZE;
	size = { 22,13 };
	CreateBoxes();

	SetCallbacks();
}
Board::~Board(){}

void Board::Update(){
	sf::FloatRect viewSpace = context->window->GetViewSpace();
	viewSpace.position.x = size.x * boxSize / 2 - viewSpace.size.x / 2;
	viewSpace.position.y = size.y * boxSize / 2 - viewSpace.size.y / 2;
	context->window->SetView(sf::View(viewSpace));
}

void Board::Render(){
	for (int i = 0; i < size.x; i++) {
		for (int j = 0; j < size.y; j++) {
			boxes[i][j].Render(context->window);
		}
	}
	for (auto& itr : pokemonsPos) {
		itr.first->Render(context->window, GetBoxMiddle(itr.second));
	}
}

sf::Vector2i Board::GetMousePosition(){
	sf::Vector2i pix_mousepos = context->window->GetMousePos();
	sf::Vector2f world_mousePos = context->window->GetRenderWindow()->mapPixelToCoords(pix_mousepos);

	sf::Vector2i hoverBox((int)std::floor(world_mousePos.x / boxSize), (int)std::floor(world_mousePos.y / boxSize));
	return hoverBox;
}

sf::Vector2f Board::GetBoxMiddle(sf::Vector2i boxCoord) {
	sf::Vector2f boxPos = boxes[boxCoord.x][boxCoord.y].GetPos();
	return sf::Vector2f(boxPos.x + boxSize / 2, boxPos.y + boxSize / 2);
}

Box* Board::GetBox(sf::Vector2i boxCoord) {
	return isBoxInBoard(boxCoord) ? &boxes[boxCoord.x][boxCoord.y] : nullptr;
}

sf::Vector2i Board::GetSize() {
	return size;
}

bool Board::SetPokemonPos(Pokemon* poke, sf::Vector2i pos) {
	if (poke == nullptr)
		return false;
	if (!CheckMove(poke, pos))
		return false;
	auto itr = pokemonsPos.find(poke);
	if (itr == pokemonsPos.end()) {
		pokemonsPos.emplace(std::make_pair(poke, pos));
	}
	else {
		pokemonsPos.at(poke) = pos;
	}
	return true;
}
bool Board::MovePokemon(Pokemon* poke, sf::Vector2i move) {
	if (poke == nullptr)
		return false;
	auto itr = pokemonsPos.find(poke);
	if (itr == pokemonsPos.end())
		return false;
	return SetPokemonPos(poke, itr->second + move);
}

bool Board::CheckMove(Pokemon* poke, sf::Vector2i pos) {
	if (poke == nullptr)
		return false;
	sf::IntRect hitbox1(pos, poke->GetSize());
	for (auto& itr : pokemonsPos) {
		if (itr.first == poke)
			continue;
		sf::IntRect hitbox2(itr.second, itr.first->GetSize());
		if (hitbox1.findIntersection(hitbox2).has_value())
			return false;
	}
	sf::IntRect boardHitBox = sf::IntRect({ 0,0 }, size);
	return boardHitBox.contains(pos + poke->GetSize() - sf::Vector2i({1,1}));
}

sf::Vector2i Board::GetPokemonPosition(Pokemon* poke) {
	return pokemonsPos[poke];
}

sf::IntRect Board::GetPokemonHitbox(Pokemon* poke) {
	auto itr = pokemonsPos.find(poke);
	if (itr == pokemonsPos.end())
		return sf::IntRect();
	return sf::IntRect(itr->second, itr->first->GetSize());
}

Pokemon* Board::GetPokemonFromPos(sf::Vector2i pos){
	for (auto& itr : pokemonsPos) {
		sf::IntRect hitbox = GetPokemonHitbox(itr.first);
		if(hitbox.contains(pos))
			return itr.first;
	}
	return nullptr;
}
std::vector<Pokemon*> Board::GetPokemonCollision(sf::IntRect hitbox) {
	std::vector<Pokemon*> res;
	for (auto& itr : pokemonsPos) {
		sf::IntRect pokeHitbox = GetPokemonHitbox(itr.first);
		if (pokeHitbox.findIntersection(hitbox).has_value()) {
			res.push_back(itr.first);
		}
	}
	return res;
}

void Board::RemovePokemon(Pokemon* poke) {
	pokemonsPos.erase(pokemonsPos.find(poke));
}

void Board::Draw(sf::Shape& shape, sf::Vector2i pos){
	shape.setPosition(GetBoxMiddle(pos));
	context->window->Draw(shape);
}

void Board::SetCallbacks() {
	
}

void Board::CreateBoxes() {
	for (int i = 0; i < size.x; i++) {
		boxes.push_back({});
		for (int j = 0; j < size.y; j++) {
			boxes[i].emplace_back(sf::Vector2f(i * boxSize, j * boxSize), sf::Vector2i(i, j), boxSize);
		}
	}
}

bool Board::isBoxInBoard(sf::Vector2i boxCoord) {
	return boxCoord.x >= 0 && boxCoord.x < size.x
		&& boxCoord.y >= 0 && boxCoord.y < size.y;
}
