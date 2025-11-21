#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <map>

#include "glm/glm.hpp"
#include "json.hpp"

using json = nlohmann::json;

//Enum representing all supported poke type
enum PokeType {
	Fire, Water, Grass, NonePokeType
};
//Convert a string into a PokeType, if the string is unknown, return PokeType::NonePokeType
PokeType GetPokeTypeFromString(const std::string& type);
//Convert a PokeType into a string
std::string GetStringFromPokeType(PokeType type);
//Getting the color associated to the poke type (hard coded)
glm::vec4 GetColorFromPokeType(PokeType type);

//Enum representing the two move category type
enum CatType{Spe, Phy, NoneCatType};
//Convert a string into a CatType, if the string is unknown, return CatType::NoneCatType
CatType GetCatTypeFromString(const std::string& type);

//Store the stats of a pokemon
struct PokemonStats {
	int hp;
	int atk;
	int def;
	int spAtk;
	int spDef;
	int speed;
	int move;
};

//Store the data relative to a move
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

//Store the data relative to a pokemon
struct PokemonData {
	int id;
	std::string name;
	std::pair<PokeType, PokeType> types;
	PokemonStats stats;
	std::string sprite;
	glm::ivec2 size;
	std::vector<const MoveData*> movePool;
};

//This class holds all the data relative to the game
class DataManager {
public:
	DataManager();
	~DataManager();

	//Get a const reference to a move data, according to its id
	const MoveData* GetMoveData(int id);
	//Get a const reference to a pokemon data, according to its id
	const PokemonData* GetPokemonData(int id);
	//Get a const reference to a pokemon data, according to its name
	const PokemonData* GetPokemonData(const std::string& name);
	//Get the const reference to the map of all the stored pokemon
	const std::map<int, PokemonData>& GetAllPokemonData();

private:
	void LoadData();
	
	void LoadPokemonData();
	void LoadMoveData();

	void LoadMoveFromFile(const std::string& file);
	void LoadPokemonFromFile(const std::string& file);

	std::map<int, MoveData> moveData;
	std::map<int, PokemonData> pokemonsData;

	std::string movesDir;
	std::string pokemonsDir;
};
