#pragma once

#include <string>
#include "Window.h"
#include "GameData.h"

//This structure stores the state of a particular move of a pokemon
struct PokemonMove {
	PokemonMove(const MoveData* l_data): data(l_data){
		pp = data->pp;
	}
	const MoveData* data;
	int pp;
};

class APlayer;

/*This class stores the status of an instance of a particular pokemon*/
class PokemonStatus {
public:
	PokemonStatus(const PokemonData* l_data);
	~PokemonStatus();

	int GetStat(Stat stat);
	int GetHealth();
	void TakeDamages(int damages);

	PokemonMove* movePool[Constants::NB_MOVES_MAX_BY_POKE];
	const PokemonData* data;

private:
	int level;
	int EV[Constants::NB_STATS];
	int IV[Constants::NB_STATS];

	int damages;
};

class Pokemon {
public:
	Pokemon(const PokemonData* data, APlayer* l_trainer,
		ModelManager* modelMgr, ShaderManager* shaderMgr, glm::vec3 l_color);
	virtual ~Pokemon();

	//Render method for the Pokemon, draw it in win at grid position pos
	void Render(Window* win, glm::ivec2 pos);
	//Render method for the Pokemon, draw it in win at real position pos
	void Render(Window* win, glm::vec3 pos);
	
	/*Perform an intersection test with a given ray, describes by ray_origin and ray_direction
	Return if there is an intersection between the Pokemon drawable and the ray
	Store in intersection_distance the distance between the ray and the Pokemon drawable
	*/
	bool TestRayIntersection(
		const glm::vec3& ray_origin,
		const glm::vec3& ray_direction,
		float& intersection_distance);

	//Getters and setters

	int GetHealth();
	int GetMaxHealth();
	glm::ivec2 GetSize();
	int GetMoveRange();
	/*Return the trainer of the pokemon, nullptr when the pokemon is wild*/
	APlayer* GetTrainer();
	/*Set the trainer of the pokemon*/
	void SetTrainer(APlayer* l_trainer);
	std::string GetName();
	bool IsKO();
	/*Return a pointer to the i-th move of this pokemon(0 <= i <= 3) 
	If he has no such move return nullptr*/
	PokemonMove* GetMove(int i);
	std::pair<PokeType, PokeType> GetType();
	void SetMovePool(int i, const MoveData* l_data);
	PokemonStatus& GetStatus();

	//Take l_damages damages
	void TakeDamages(int l_damages);

private:
	ModelManager* modelMgr;

	glm::vec3 color;

	Model* model;
	Drawable* sprite;

	Drawable* OBB;

	PokemonStatus status;

	APlayer* trainer;
};