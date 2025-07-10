#include "GameSystem.h"

GameSystem::GameSystem(SharedContext* l_context)
	: context(l_context), board(glm::vec2(20, 20), l_context){
	l_context->gameSystem = this;
}
GameSystem::~GameSystem(){}

void GameSystem::Update(double dt){
	board.Update(dt);
}

void GameSystem::Render(){
	board.Draw();
}