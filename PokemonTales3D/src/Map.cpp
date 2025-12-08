#include "Map.h"

glm::ivec3 Map::MapIndFromPos(glm::vec3 pos) {
	return glm::ivec3(pos.x / Constants::BLOCK_SIZE - (pos.x < 0 ? 1 : 0),
		pos.y / Constants::BLOCK_SIZE - (pos.y < 0 ? 1 : 0),
		pos.z / Constants::BLOCK_SIZE - (pos.z < 0 ? 1 : 0));
}

Map::Map(SharedContext* l_context, const std::string& file)
	: context(l_context), instanceBlocks(glm::ivec2(0,0), glm::ivec2(0,0))
{
	context->map = this;

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
		blocks.reserve(j.size());
		for (json blockData : j) {
			blocks.emplace_back(context->modelManager, context->shaderManager, blockData);
		}
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
	
	std::vector<BlockInstance> tabI;
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

			glm::ivec2 orientation = glm::ivec2(0,1);
			if (elem.contains("Orientation")) {
				orientation.x = elem["Orientation"][0];
				orientation.y = elem["Orientation"][1];
			}

			for (int i = areaPos.x; i < areaPos.x + areaSize.x; i++) {
				for (int j = areaPos.y; j < areaPos.y + areaSize.y; j++) {
					for (int k = areaPos.z; k < areaPos.z + areaSize.z; k++) {
						Transform* t = new Transform();
						
						t->Move(glm::vec3(Constants::BLOCK_SIZE * i, Constants::BLOCK_SIZE * j, Constants::BLOCK_SIZE * k)
							+ glm::vec3(Constants::BLOCK_SIZE / 2.0f));

						if(orientation == glm::ivec2(0,-1))
							t->Rotate(glm::vec3(0.0f, 180.0f, 0.0f));
						else if (orientation == glm::ivec2(1, 0))
							t->Rotate(glm::vec3(0.0f, 90.0f, 0.0f));
						else if (orientation == glm::ivec2(-1, 0))
							t->Rotate(glm::vec3(0.0f, -90.0f, 0.0f));

						T.push_back(t);

						BlockInstance inst(block, glm::ivec3(i,j,k));
						inst.orientation = orientation;
						tabI.push_back(inst);
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
	for(auto& i : tabI){
		instanceBlocks.AddBlock(i);
	}
}

void Map::Unload(){
	for (auto& i : drawableBlocks)
		i.DeleteTransforms();
	drawableBlocks.clear();
}

bool Map::GetCollision(glm::vec3 pos) {
	const BlockInstance* collision = GetBlockCollision(pos);
	return collision != nullptr;
}
bool Map::GetIsOnCube(glm::vec3 pos) {
	return GetBlockCollision(
		pos - glm::vec3(0.0f, Constants::BLOCK_SIZE - 0.01f, 0.0f)
		, Block::Type::Cube) != nullptr;
}

const BlockInstance* Map::GetBlockCollision(glm::vec3 pos) {
	return instanceBlocks.GetBlock(Map::MapIndFromPos(pos));
}
const BlockInstance* Map::GetBlockCollision(glm::vec3 pos, Block::Type type) {
	const BlockInstance* b = instanceBlocks.GetBlock(Map::MapIndFromPos(pos));
	if (b == nullptr || b->block->GetType() != type)
		return nullptr;
	return b;
}

glm::vec3 Map::ReactMovment(glm::vec3 prevPos, glm::vec3 newPos) {
	const BlockInstance* collision = GetBlockCollision(newPos);
	if (collision == nullptr)
		return newPos;
	if (collision->block->GetType() == Block::Type::Cube)
		return ReactMovmentCube(prevPos, newPos);
	else if (collision->block->GetType() == Block::Type::Stair)
		return ReactMovmentStair(prevPos, newPos, collision);
}

glm::vec3 Map::ReactMovmentCube(glm::vec3 prevPos, glm::vec3 newPos) {
	glm::vec3 wx = glm::vec3(prevPos.x, newPos.y, newPos.z);
	if (!GetCollision(wx))
		return wx;
	glm::vec3 wy = glm::vec3(newPos.x, prevPos.y, newPos.z);
	if (!GetCollision(wy))
		return wy;
	glm::vec3 wz = glm::vec3(newPos.x, newPos.y, prevPos.z);
	if (!GetCollision(wz))
		return wz;
	glm::vec3 wxz = glm::vec3(prevPos.x, newPos.y, prevPos.z);
	if (!GetCollision(wxz))
		return wxz;
	glm::vec3 wxy = glm::vec3(prevPos.x, prevPos.y, newPos.z);
	if (!GetCollision(wxy))
		return wxy;
	glm::vec3 wyz = glm::vec3(newPos.x, prevPos.y, prevPos.z);
	if (!GetCollision(wyz))
		return wyz;
	return prevPos;
	
}
glm::vec3 Map::ReactMovmentStair(glm::vec3 prevPos, glm::vec3 newPos, const BlockInstance* collision) {
	float detection = 0.1f;
	
	glm::vec3 blockPos = glm::vec3(collision->pos.x, collision->pos.y, collision->pos.z) * Constants::BLOCK_SIZE;
	if (collision->orientation.x == 0) {
		float pos = newPos.z - blockPos.z;
		float fz = (collision->orientation.y >= 0 ? Constants::BLOCK_SIZE - pos : pos);
		if (fz - newPos.y <= detection) {
			return glm::vec3(newPos.x, blockPos.y + fz + 0.05f, newPos.z);
		}
	} else {
		float pos = newPos.x - blockPos.x;
		float fz = (collision->orientation.x >= 0 ? 1 - pos : pos);
		if (blockPos.y + fz - newPos.y <= detection) {
			return glm::vec3(newPos.x, blockPos.y + fz + 0.05f, newPos.z);
		}
	}
	return ReactMovmentCube(prevPos, newPos);
}