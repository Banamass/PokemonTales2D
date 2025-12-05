#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <fstream>

#include "Window.h"
#include "json.hpp"

using json = nlohmann::json;

class Block : public Drawable {
public:
	Block(ModelManager* l_modelMgr, ShaderManager* l_shaderMgr, const json& data);
	~Block();

	std::string GetName();

protected:
	void LoadData(const json& data);

	std::string blockName;

	ModelManager* modelMgr;
	std::string modelName;
	ShaderManager* shaderMgr;
};

struct BlockInstance {
	BlockInstance(Block* l_block, glm::ivec3 l_pos);

	Block* block;
	glm::ivec3 pos;
};

class BlockStorage {
public:
	BlockStorage(glm::ivec2 minPos, glm::ivec2 maxPos);
	virtual ~BlockStorage();

	//Reset the block storage with new min and max pos
	void Reset(glm::ivec2 minPos, glm::ivec2 maxPos);

	//Add the block instance block
	bool AddBlock(BlockInstance block);
	//Get the block instance at a given pos in the map grid
	const BlockInstance* GetBlock(glm::ivec3 pos);

private:
	/*Store the blocks in a matrix with coords (x, z), x and z within [minPos, maxPos]
	Each element of this matrix is a map with each entry : (y-coord -> BlockInstance) */

	glm::ivec2 GetInd(glm::ivec2 pos);

	std::vector<std::vector<std::unordered_map<int, BlockInstance>>> blocks;
	glm::ivec2 minPos;
	glm::ivec2 maxPos;
};