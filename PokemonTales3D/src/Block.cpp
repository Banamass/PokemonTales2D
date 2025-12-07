#include "Block.h"

/*--------------Block--------------*/

Block::Block(ModelManager* l_modelMgr, ShaderManager* l_shaderMgr, const json& data)
	: Drawable(), modelMgr(l_modelMgr), shaderMgr(l_shaderMgr), modelName(""), type(Type::Cube)
{
	LoadData(data);
}
Block::~Block(){
	std::cout << "Destroy " << blockName << std::endl;
	if(modelName != "")
		modelMgr->ReleaseResource(modelName);
}

std::string Block::GetName() {
	return blockName;
}

void Block::LoadData(const json& data) {
	blockName = data["Name"];

	modelName = data["Model"];
	std::string shaderName = "InstancedModelShader";

	SetModel(modelMgr->RequireGetResource(modelName));
	SetShader(shaderMgr->GetShader(shaderName));

	json matData = data["Material"];
	Drawable::Material mat;
	mat.SetLightningColor(ColorFromRGB(matData["Color"][0], matData["Color"][1], matData["Color"][2])
		, matData["AmbientFact"], matData["SpecularFact"]);
	SetMaterial(mat);

	std::string sType = data["Type"];
	if (sType == "Cube")
		type = Type::Cube;
	if (sType == "Stair") 
		type = Type::Stair;
}

Block::Type Block::GetType() {
	return type;
}

/*--------------BlockInstance--------------*/

BlockInstance::BlockInstance(Block* l_block, glm::ivec3 l_pos)
: block(l_block), pos(l_pos), orientation(0,1){}

/*--------------BlockStorage--------------*/

BlockStorage::BlockStorage(glm::ivec2 l_minPos, glm::ivec2 l_maxPos) {
	Reset(l_minPos, l_maxPos);
}

BlockStorage::~BlockStorage() {}

void BlockStorage::Reset(glm::ivec2 l_minPos, glm::ivec2 l_maxPos) {
	minPos = l_minPos;
	maxPos = l_maxPos;
	blocks.clear();

	for (int i = minPos.x; i <= maxPos.x; i++) {
		auto& itr = blocks.emplace_back();
		for (int j = minPos.y; j <= maxPos.y; j++) {
			itr.emplace_back();
		}
	}
}

bool BlockStorage::AddBlock(BlockInstance block) {
	glm::vec2 ind = GetInd(glm::vec2(block.pos.x, block.pos.z));
 	if (ind.x == -1)
		return false;
	auto itr = blocks[ind.x][ind.y].find(block.pos.y);
	if (itr != blocks[ind.x][ind.y].end())
		return false;
	blocks[ind.x][ind.y].insert(std::make_pair(block.pos.y, block));
	return true;
}
const BlockInstance* BlockStorage::GetBlock(glm::ivec3 pos) {
	glm::vec2 ind = GetInd(glm::vec2(pos.x, pos.z));
	if (ind.x == -1)
		return nullptr;
	auto itr = blocks[ind.x][ind.y].find(pos.y);
	if (itr == blocks[ind.x][ind.y].end())
		return nullptr;
	return &itr->second;
}

glm::ivec2 BlockStorage::GetInd(glm::ivec2 pos) {
	if (!(pos.x >= minPos.x && pos.x <= maxPos.x &&
		pos.y >= minPos.y && pos.y <= maxPos.y))
		return glm::ivec2(-1, -1);
	return pos - minPos;
}