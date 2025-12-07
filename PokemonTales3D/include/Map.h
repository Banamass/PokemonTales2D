#pragma once

#include "EventManager.h"
#include "Camera.h"
#include "Block.h"
#include <unordered_map>

class Map {
public:
	static glm::ivec3 MapIndFromPos(glm::vec3 pos);

	Map(SharedContext* l_context, const std::string& file);
	~Map();

	void Update(double dt);
	void Render();
	Block* GetBlock(const std::string& name);

	/*Return if there is a collision between the pos and an element of the map*/
	bool GetCollision(glm::vec3 pos);
	/*Return the block which has a collision with the given pos, or nullptr is there is none*/
	const BlockInstance* GetBlockCollision(glm::vec3 pos);
	/*Return the position of an object who makes a movment from prevPos to newPos
	Taking into account all map elements*/
	glm::vec3 ReactMovment(glm::vec3 prevPos, glm::vec3 newPos);

private:
	glm::vec3 ReactMovmentCube(glm::vec3 prevPos, glm::vec3 newPos);
	glm::vec3 ReactMovmentStair(glm::vec3 prevPos, glm::vec3 newPos, const BlockInstance* collision);

	void LoadBlocks(const std::string& fileName);
	void Load(const std::string& file);
	void Unload();

	SharedContext* context;

	std::vector<Block> blocks;
	std::vector<DrawableInstanced> drawableBlocks;
	BlockStorage instanceBlocks;
};