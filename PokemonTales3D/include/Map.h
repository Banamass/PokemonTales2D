#pragma once

#include "EventManager.h"
#include "Camera.h"
#include "Block.h"
#include "PlayerBody.h"
#include <unordered_map>

class Map {
public:
	Map(SharedContext* l_context, const std::string& file);
	~Map();

	void Update(double dt);
	void Render();
	Block* GetBlock(const std::string& name);

private:
	void LoadBlocks(const std::string& fileName);
	void Load(const std::string& file);
	void Unload();

	SharedContext* context;

	std::vector<Block> blocks;
	std::vector<DrawableInstanced> drawableBlocks;
	BlockStorage instanceBlocks;
};