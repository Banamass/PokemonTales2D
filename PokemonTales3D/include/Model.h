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

//Store mesh vertice infos
struct Vertex{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

//Store mesh texture infos
struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

/*This class represents a mesh, which is a set of vertices, indices and textures */
class Mesh {
public:
	std::vector<Vertex> vertices; //All the vertices contained in the mesh
	std::vector<unsigned int> indices; //The concrete order of vertices for drawing
	std::vector<Texture> textures; //All the texture contained in the mesh

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
		std::vector<Texture> textures);

	//Draw the mesh with a given shader, and camera and model matrix
	void Draw(Shader* shader, glm::mat4& cameraMatrix, glm::mat4& modelMatrix);
	//(depreciated)
	void DrawInstanced(Shader* shader, glm::mat4 cameraMatrix, const std::vector<Transform*>& instanceTransform);
	//Draw several instance of the mesh with a given shader and cameraMatrix, according to the drawableInstanced object
	void DrawInstanced(Shader* shader, glm::mat4 cameraMatrix, DrawableInstanced* drawable);

	/*Return (aabbMin, aabbMax), which represent the min and max coords on each axis of the mesh*/
	std::pair<glm::vec3, glm::vec3> GetAABBMinMax();

private:
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};

/*Load a texture in the OpenGL context, given a name and a directory, and return the 
OpenGL texture id of the loaded texture */
unsigned int TextureFromFile(const char* name, std::string directory);

//This class represents a model which is a set of mesh
class Model {
public:
	Model(const char* path) {
		loadModel(path);
	}

	//Draw the model with a given shader, and camera and model matrix
	void Draw(Shader* shader, glm::mat4 cameraMatrix, glm::mat4 modelMatrix);
	//(depreciated)
	void DrawInstanced(Shader* shader, glm::mat4 cameraMatrix, const std::vector<Transform*>& instanceTransform);
	//Draw several instance of the mesh with a given shader and cameraMatrix, according to the drawableInstanced object
	void DrawInstanced(Shader* shader, glm::mat4 cameraMatrix, DrawableInstanced* drawable);

	/*Return (aabbMin, aabbMax), which represents the min and max coords on each axis of the model*/
	std::pair<glm::vec3, glm::vec3> GetAABBMinMax();

private:
	void SetAABBMinMax();
	glm::vec3 AABBMin;
	glm::vec3 AABBMax;

	std::vector<Mesh> meshes;
	std::string directory;

	std::unordered_map<std::string, Texture> texturesLoaded;

	/*Load a model from a file from one of these formats:
	- Wavefront*/
	void loadModel(std::string path);

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat
		, aiTextureType type, std::string typeName);
};

/* This class manages all models loaded by the game  */
class ModelManager : public ResourceManager<ModelManager, Model> {
public:
	ModelManager(SharedContext* context) : ResourceManager("Resources/Models/models.cfg") {
		context->modelManager = this;
	}

	//Load a model with given arguments
	Model* Load(const std::vector<std::string>* l_args)
	{
		if (l_args->size() < 1)
			return nullptr;
		Model* res = new Model(l_args->at(0).data());
		return res;
	}
};

/* This class represent a cubemap 3D object */
class Cubemap {
public:
	/*Faces contains for each face the name of the file representing that face, all these file are in dir
	Face files are : "right.ext","left.ext","up.ext","down.ext","front.ext","back.ext"
	*/
	Cubemap(Shader* l_shader, std::string dir = "", std::string ext = "png");
	//Faces contains for each face the name of the file representing that face, all these file are in dir
	Cubemap(Shader* l_shader, std::vector<std::string>& faces, std::string dir = "");
	~Cubemap();

	//Draw the cubemap according to the transform matrix
	void Draw(glm::mat4 transform);

private:
	void Setup(std::vector<std::string>& faces, std::string dir);

	Shader* shader;
	unsigned int textures;
	unsigned int VAO;
	unsigned int VBO;
};