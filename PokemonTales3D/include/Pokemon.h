#pragma once

#include <string>
#include "Window.h"
#include "GameData.h"

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
		ShaderManager* shaderMgr, glm::vec3 l_color);
	virtual ~Pokemon();

	void Render(Window* win, glm::ivec2 pos);

	bool TestRayIntersection(
		const glm::vec3& ray_origin,
		const glm::vec3& ray_direction,
		float& interstion_distance);

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

	void TakeDamages(float l_damages);

private:
	glm::vec3 color;

	Model model;
	Drawable sprite;

	Drawable OBB;

	float health;
	PokemonMove* movePool[4];

	const PokemonData* data;

	APlayer* trainer;
};