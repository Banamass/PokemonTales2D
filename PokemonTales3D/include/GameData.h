#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>

#include "glm/glm.hpp"
#include "json.hpp"

using json = nlohmann::json;

enum PokeType {
	Fire, Water, None
};
PokeType GetPokeTypeFromString(const std::string& type);

enum CatType{Spe, Phy};
CatType GetCatTypeFromString(const std::string& type);

struct PokemonStats {
	int hp;
	int atk;
	int def;
	int spAtk;
	int spDef;
	int speed;
	int move;
};

struct MoveData {
	int id;
	std::string name;
	PokeType type;
	CatType cat;
	int power;
	int acc;
	int pp;
	std::string effect;
	int prob;
	glm::ivec2 hitZone;
	int range;
};

struct PokemonData {
	int id;
	std::string name;
	std::pair<PokeType, PokeType> types;
	PokemonStats stats;
	std::string sprite;
	glm::ivec2 size;
	std::vector<const MoveData*> movePool;
};

class DataManager {
public:
	DataManager();
	~DataManager();

	const MoveData* GetMoveData(int id);
	const PokemonData* GetPokemonData(int id);

private:
	void LoadData();
	
	void LoadPokemonData();
	void LoadMoveData();

	void LoadMoveFromFile(const std::string& file);
	void LoadPokemonFromFile(const std::string& file);

	std::vector<MoveData> moveData;
	std::vector<PokemonData> pokemonsData;

	std::string movesDir;
	std::string pokemonsDir;
};
