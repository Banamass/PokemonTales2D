#pragma once

#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include <utility>
#include "stb_image.h"
#include "ResourceManager.h"

class Transform;
class DrawableInstanced;

struct Vertex{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
		std::vector<Texture> textures);

	void Draw(Shader* shader, glm::mat4& cameraMatrix, glm::mat4& modelMatrix);
	void DrawInstanced(Shader* shader, glm::mat4 cameraMatrix, const std::vector<Transform*>& instanceTransform);
	void DrawInstanced(Shader* shader, glm::mat4 cameraMatrix, DrawableInstanced* drawable);

	/*Return (aabbMin, aabbMax)*/
	std::pair<glm::vec3, glm::vec3> GetAABBMinMax();

private:
	unsigned int VAO, VBO, EBO;

	void setupMesh();

};

unsigned int TextureFromFile(const char* name, std::string directory);

class Model {
public:
	Model(const char* path) {
		loadModel(path);
	}

	void Draw(Shader* shader, glm::mat4 cameraMatrix, glm::mat4 modelMatrix);
	void DrawInstanced(Shader* shader, glm::mat4 cameraMatrix, const std::vector<Transform*>& instanceTransform);
	void DrawInstanced(Shader* shader, glm::mat4 cameraMatrix, DrawableInstanced* drawable);

	/*Return (aabbMin, aabbMax)*/
	std::pair<glm::vec3, glm::vec3> GetAABBMinMax();

private:
	void SetAABBMinMax();
	glm::vec3 AABBMin;
	glm::vec3 AABBMax;

	std::vector<Mesh> meshes;
	std::string directory;

	std::unordered_map<std::string, Texture> texturesLoaded;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat
		, aiTextureType type, std::string typeName);
};

class ModelManager : public ResourceManager<ModelManager, Model> {
public:
	ModelManager(SharedContext* context) : ResourceManager("Resources/Models/models.cfg") {
		context->modelManager = this;
	}

	Model* Load(const std::vector<std::string>* l_args)
	{
		if (l_args->size() < 1)
			return nullptr;
		Model* res = new Model(l_args->at(0).data());
		return res;
	}
};

class Cubemap {
public:
	Cubemap(Shader* l_shader, std::vector<std::string>& faces, std::string dir = "");
	~Cubemap();

	void Draw(glm::mat4 transform);

private:
	Shader* shader;
	unsigned int textures;
	unsigned int VAO;
	unsigned int VBO;
};