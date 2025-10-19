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

class Pokemon {
public:
	Pokemon(const PokemonData* data, APlayer* l_trainer,
		ModelManager* modelMgr, ShaderManager* shaderMgr, glm::vec3 l_color);
	virtual ~Pokemon();

	//Render method for the Pokemon, draw it in win at position pos
	void Render(Window* win, glm::ivec2 pos);

	
	/*Perform an intersection test with a given ray, describes by ray_origin and ray_direction
	Return if there is an intersection between the Pokemon drawable and the ray
	Store in intersection_distance the distance between the ray and the Pokemon drawable
	*/
	bool TestRayIntersection(
		const glm::vec3& ray_origin,
		const glm::vec3& ray_direction,
		float& intersection_distance);

	//Getters and setters

	float GetHealth();
	float GetMaxHealth();
	glm::ivec2 GetSize();
	int GetMoveRange();
	APlayer* GetTrainer();
	std::string GetName();
	bool IsKO();
	PokemonMove* GetMove(int i);
	std::pair<PokeType, PokeType> GetType();
	void SetMovePool(int i, const MoveData* l_data);

	//Take l_damages damages
	void TakeDamages(float l_damages);

private:
	ModelManager* modelMgr;

	glm::vec3 color;

	Model* model;
	Drawable* sprite;

	Drawable* OBB;

	float health;
	PokemonMove* movePool[4];

	const PokemonData* data;

	APlayer* trainer;
};