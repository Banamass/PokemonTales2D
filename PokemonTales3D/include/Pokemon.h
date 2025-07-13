#pragma once

#include <string>
#include "Window.h"

class APlayer;

class Pokemon {
public:
	Pokemon(const std::string& l_name, glm::ivec2 l_size, APlayer* l_trainer);
	virtual ~Pokemon();

	void Render(Window* win, glm::ivec2 pos);

	glm::ivec2 GetSize();
	int GetMoveRange();
	void SetMoveRange(int l_range);
	APlayer* GetTrainer();
	std::string GetName();
	bool IsKO();

	void TakeDamages(float l_damages);

private:
	glm::ivec2 size; //nb box takes
	int moveRange;
	float health;
	std::string name;

	APlayer* trainer;
};