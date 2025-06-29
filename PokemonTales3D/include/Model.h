#pragma once

#include "Shader.h"
#include "Window.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include "stb_image.h"

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

	void Draw(Shader& shader, glm::mat4 transform);

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

	void Draw(Shader& shader, glm::mat4 transform);

private:
	std::vector<Mesh> meshes;
	std::string directory;

	std::unordered_map<std::string, Texture> texturesLoaded;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat
		, aiTextureType type, std::string typeName);
};

class Object {
public:
	Object();
	~Object();
	
	void SetShader(Shader* l_shader);

	void Draw(const glm::mat4& cameraMatrix);

private:
	Shader* shader;

	unsigned int VBO;
	unsigned int VAO;

	glm::mat4 modelMatrix;
};