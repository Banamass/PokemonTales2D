#pragma once

#include <string>
#include "Window.h"

class APlayer;

class Pokemon {
public:
	Pokemon(const std::string& l_name, glm::ivec2 l_size, APlayer* l_trainer,
		ShaderManager* shaderMgr, glm::vec3 l_color);
	virtual ~Pokemon();

	void Render(Window* win, glm::ivec2 pos);

	bool TestRayIntersection(
		const glm::vec3& ray_origin,
		const glm::vec3& ray_direction,
		float& interstion_distance);

	glm::ivec2 GetSize();
	float GetHeight();
	int GetMoveRange();
	void SetMoveRange(int l_range);
	APlayer* GetTrainer();
	std::string GetName();
	bool IsKO();

	void TakeDamages(float l_damages);

private:
	glm::vec3 color;

	Model model;
	Drawable sprite;

	Drawable OBB;

	glm::ivec2 size; //nb box takes
	float height;
	int moveRange;
	float health;
	std::string name;

	APlayer* trainer;
};