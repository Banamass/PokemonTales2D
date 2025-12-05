#include "World.h"

World::World(SharedContext* l_context)
	: context(l_context), map(l_context, "Resources\\GameData\\Map\\map.json")
{
	
}
World::~World(){
}

void World::Update(double dt){
	map.Update(dt);
}
void World::Render(){
	map.Render();
}