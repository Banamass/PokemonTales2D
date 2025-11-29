#include "GameData.h"

PokeType GetPokeTypeFromString(const std::string& type){
	if (type == "Fire")
		return PokeType::Fire;
	if (type == "Water")
		return PokeType::Water;
	if (type == "Grass")
		return PokeType::Grass;
	else
		return PokeType::NonePokeType;
}
std::string GetStringFromPokeType(PokeType type) {
	if (type == PokeType::Fire)
		return "Fire";
	if (type == PokeType::Water)
		return "Water";
	if (type == PokeType::Grass)
		return "Grass";
	else
		return "None";
}

std::string GetStringFromStat(Stat stat) {
	if (stat == Stat::Atk)
		return "Atk";
	if (stat == Stat::Def)
		return "Def";
	if (stat == Stat::SpAtk)
		return "SpAtk";
	if (stat == Stat::SpDef)
		return "SpDef";
	if (stat == Stat::Hp)
		return "Hp";
	if (stat == Stat::Speed)
		return "Speed";
	if (stat == Stat::Move)
		return "Move";
}

glm::vec4 GetColorFromPokeType(PokeType type) {
	if (type == PokeType::Fire)
		return glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	if (type == PokeType::Water)
		return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	if (type == PokeType::Grass)
		return glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	else
		return glm::vec4(0.0f);
}

CatType GetCatTypeFromString(const std::string& type) {
	if (type == "Spe")
		return CatType::Spe;
	else if (type == "Phy")
		return CatType::Phy;
	else
		return CatType::NoneCatType;
}
std::string GetStringFromCatType(CatType type) {
	if (type == CatType::Spe)
		return "Spe";
	else if (type == CatType::Phy)
		return "Phy";
	else
		return "None";
}

DataManager::DataManager(){
	movesDir = "Resources\\GameData\\Moves\\";
	pokemonsDir = "Resources\\GameData\\Pokemons\\";
	LoadData();
}
DataManager::~DataManager(){}

void DataManager::LoadData(){
	LoadMoveData();
	LoadPokemonData();
}

void DataManager::LoadMoveData() {
	std::string fName = movesDir + "Moves.json";
	std::ifstream file(fName);
	if (!file.is_open()) {
		std::cout << "Failed to load file " << fName << std::endl;
		return;
	}
	json j;
	try {
		file >> j;
	}
	catch (json::exception ex) {
		std::cout << ex.what() << std::endl;
		return;
	}

	for (const std::string& mFile : j) {
		LoadMoveFromFile(mFile);
	}
}
void DataManager::LoadPokemonData(){
	std::string fName = pokemonsDir + "Pokemons.json";
	std::ifstream file(fName);
	if (!file.is_open()) {
		std::cout << "Failed to load file " << fName << std::endl;
		return;
	}
	json j;
	try{
		file >> j;
	}
	catch (json::exception ex) {
		std::cout << ex.what() << std::endl;
		return;
	}
	

	for (const std::string& pFile : j) {
		LoadPokemonFromFile(pFile);
	}
}

void DataManager::LoadMoveFromFile(const std::string& file){
	std::string fileName = movesDir + file;
	std::ifstream f(fileName);
	if (!f.is_open()) {
		std::cout << "Failed to load file " << fileName << std::endl;
		return;
	}
	json j;
	try {
		f >> j;
	}
	catch (json::exception ex) {
		std::cout << ex.what() << std::endl;
		return;
	}

	int id = j["ID"].get<int>();
	MoveData& d = moveData.insert(std::make_pair(id, MoveData())).first->second;
	d.id = j["ID"].get<int>();
	d.name = j["Name"].get<std::string>();
	d.type = GetPokeTypeFromString(j["Type"].get<std::string>());
	d.cat = GetCatTypeFromString(j["Cat"].get<std::string>());
	d.power = j["Power"].get<int>();
	d.acc = j["Acc."].get<int>();
	d.pp = j["PP"].get<int>();
	d.effect = j["Effect"].get<std::string>();
	d.prob = j["Prob"].get<int>();
	d.hitZone.x = j["HitZone"][0];
	d.hitZone.y = j["HitZone"][1];
	d.range = j["Range"].get<int>();
}
void DataManager::LoadPokemonFromFile(const std::string& file){
	std::string fileName = pokemonsDir + file;
	std::ifstream f(fileName);
	if (!f.is_open()) {
		std::cout << "Failed to load file " << fileName << std::endl;
		return;
	}
	json j;
	try {
		f >> j;
	}
	catch (json::exception ex) {
		std::cout << ex.what() << std::endl;
		return;
	}

	int id = j["ID"].get<int>();
	PokemonData& d = pokemonsData.insert(std::make_pair(id, PokemonData())).first->second;
	d.id = j["ID"].get<int>();
	d.name = j["Name"].get<std::string>();
	d.types.first = GetPokeTypeFromString(j["Type"][0]);
	d.types.second = j["Type"].size() < 2 ? PokeType ::NonePokeType : GetPokeTypeFromString(j["Type"][1]);
	d.stats[Stat::Hp] = j["Stats"]["HP"].get<int>();
	d.stats[Stat::Atk] = j["Stats"]["Attack"].get<int>();
	d.stats[Stat::Def] = j["Stats"]["Defense"].get<int>();
	d.stats[Stat::SpAtk] = j["Stats"]["Sp.Atk"].get<int>();
	d.stats[Stat::SpDef] = j["Stats"]["Sp.Def"].get<int>();
	d.stats[Stat::Speed] = j["Stats"]["Speed"].get<int>();
	d.stats[Stat::Move] = j["Stats"]["Move"].get<int>();
	d.sprite = j["Sprite"].get<std::string>();
	d.size.x = j["Size"][0];
	d.size.y = j["Size"][1];
	for (int moveId : j["AttackPool"]) {
		if (const MoveData* m = GetMoveData(moveId))
			d.movePool.push_back(m);
	}
}

const MoveData* DataManager::GetMoveData(int id) { 
	auto itr = moveData.find(id);
	if (itr == moveData.end()) {
		std::cout << "Move with id " << id << " doesn't exist" << std::endl;
		return nullptr;
	}
	return &itr->second; 
}
const PokemonData* DataManager::GetPokemonData(int id) {
	auto itr = pokemonsData.find(id);
	if (itr == pokemonsData.end()) {
		std::cout << "Pokemon with id " << id << " doesn't exist" << std::endl;
		return nullptr;
	}
	return &itr->second;
}

const PokemonData* DataManager::GetPokemonData(const std::string& name) {
	for (auto& data : pokemonsData) {
		if (data.second.name == name) {
			return &data.second;
		}
	}
	return nullptr;
}

const std::map<int, PokemonData>& DataManager::GetAllPokemonData() {
	return pokemonsData;
}