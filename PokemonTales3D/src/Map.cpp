#include "Map.h"

Map::Map(SharedContext* l_context, const std::string& file)
	: context(l_context), instanceBlocks(glm::ivec2(0,0), glm::ivec2(0,0))
{
	LoadBlocks("Resources\\GameData\\Map\\blocks.json");
	Load(file);
}
Map::~Map(){
	Unload();
}

void Map::Update(double dt){}
void Map::Render(){
	for(auto& i : drawableBlocks)
		context->win->DrawInstanced(&i);
}

Block* Map::GetBlock(const std::string& name) {
	for (auto& b : blocks) {
		if (b.GetName() == name)
			return &b;
	}
	return nullptr;
}

void Map::LoadBlocks(const std::string& fileName) {
	std::ifstream file(fileName);
	if (!file.is_open())
		std::cout << "Unable to open " << fileName << std::endl;

	json j;
	try {
		file >> j;
		for (json blockData : j)
			blocks.emplace_back(context->modelManager, context->shaderManager, blockData);
	}
	catch (json::exception ex) {
		std::cout << ex.what() << std::endl;
	}
}

void Map::Load(const std::string& fileName){
	Unload();

	std::ifstream file(fileName);
	if (!file.is_open())
		std::cout << "Unable to open " << fileName << std::endl;

	std::unordered_map<Block*, std::vector<Transform*>> tabT;
	
	std::vector<std::pair<Block*, glm::ivec3>> tabP;
	glm::ivec2 minPos(0,0);
	glm::ivec2 maxPos(0,0);

	json j;
	try {
		file >> j;
		for (json elem : j) {
			Block* block = GetBlock(elem["Block"]);
			if (block == nullptr) {
				std::cout << "The block " << elem["Block"] << " doesn't exist" << std::endl;
				continue;
			}

			auto itr = tabT.find(block);
			if (itr == tabT.end())
				itr = tabT.insert(std::make_pair(block, std::vector<Transform*>())).first;
			std::vector<Transform*>& T = itr->second;

			glm::ivec3 areaPos(elem["Pos"][0], elem["Pos"][1], elem["Pos"][2]);
			glm::ivec3 areaSize(elem["Size"][0], elem["Size"][1], elem["Size"][2]);
			for (int i = areaPos.x; i < areaPos.x + areaSize.x; i++) {
				for (int j = areaPos.y; j < areaPos.y + areaSize.y; j++) {
					for (int k = areaPos.z; k < areaPos.z + areaSize.z; k++) {
						Transform* t = new Transform();
						t->Move(glm::vec3(Constants::BLOCK_SIZE * i, Constants::BLOCK_SIZE * j, Constants::BLOCK_SIZE * k));
						T.push_back(t);
						tabP.push_back(std::make_pair(block, glm::ivec3(i,j,k)));
						minPos.x = std::min(minPos.x, i);
						minPos.y = std::min(minPos.y, k);
						maxPos.x = std::max(maxPos.x, i);
						maxPos.y = std::max(maxPos.y, k);
					}
				}
			}
		}
	}
	catch (json::exception ex) {
		std::cout << ex.what() << std::endl;
	}

	for (auto& itr : tabT) {
		drawableBlocks.emplace_back(itr.first, itr.second);
	}
	instanceBlocks.Reset(minPos, maxPos);
	for(auto& p : tabP){
		instanceBlocks.AddBlock(BlockInstance(p.first, p.second));
	}
}

void Map::Unload(){
	for (auto& i : drawableBlocks)
		i.DeleteTransforms();
	drawableBlocks.clear();
}