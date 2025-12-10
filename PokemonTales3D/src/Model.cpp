#include "Model.h"
#include "Drawable.h"

/*-------------------Mesh-------------------*/

Mesh::Mesh(std::vector<Vertex> l_vertices, std::vector<unsigned int> l_indices,
	std::vector<Texture> l_textures){ 
	vertices = l_vertices;
	indices = l_indices;
	textures = l_textures;

	setupMesh();
}

void Mesh::Draw(Shader* shader, glm::mat4& cameraMatrix, glm::mat4& modelMatrix) {
	shader->use();
	shader->SetUniform("transform", glm::value_ptr(cameraMatrix * modelMatrix));
	shader->SetUniform("model", glm::value_ptr(modelMatrix));
	
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if(name == "texture_specular")
			number = std::to_string(specularNr++);

		shader->SetUniform(("material." + name + number).c_str(),(float) i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	shader->unuse();
}

void Mesh::DrawInstanced(Shader* shader, glm::mat4 cameraMatrix, const std::vector<Transform*>& instanceTransform) {
	unsigned int instanceVBO;
}

void Mesh::DrawInstanced(Shader* shader, glm::mat4 cameraMatrix, DrawableInstanced* drawable) {
	if (!drawable->IsVAOSetup()) {
		glBindBuffer(GL_ARRAY_BUFFER, drawable->GetInstanceVBO());
		glBindVertexArray(VAO);
		std::size_t v4s = sizeof(glm::vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void*)(1 * v4s));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void*)(2 * v4s));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void*)(3 * v4s));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);

		drawable->VAOSetup();
	}
	shader->use();
	shader->SetUniform("cameraMatrix", glm::value_ptr(cameraMatrix));

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse") {
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader->SetUniform(("material." + name + number).c_str(), (float)i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, drawable->GetNbInstance());
	glBindVertexArray(0);
	shader->unuse();
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
}

std::pair<glm::vec3, glm::vec3> Mesh::GetAABBMinMax() {
	auto itr = vertices.begin();
	if (vertices.end() == itr)
		return std::make_pair(glm::vec3(0.0f), glm::vec3(0.0f));
	glm::vec3 AABBMin = itr->Position;
	glm::vec3 AABBMax = itr->Position;
	itr++;
	for (; itr != vertices.end(); itr++) {
		AABBMin.x = std::min(AABBMin.x, itr->Position.x);
		AABBMin.y = std::min(AABBMin.y, itr->Position.y);
		AABBMin.z = std::min(AABBMin.z, itr->Position.z);
		AABBMax.x = std::max(AABBMax.x, itr->Position.x);
		AABBMax.y = std::max(AABBMax.y, itr->Position.y);
		AABBMax.z = std::max(AABBMax.z, itr->Position.z);
	}
	return std::make_pair(AABBMin, AABBMax);
}

/*-------------------Model-------------------*/

void Model::Draw(Shader* shader, glm::mat4 cameraMatrix, glm::mat4 modelMatrix) {
	for (Mesh& mesh : meshes)
		mesh.Draw(shader, cameraMatrix, modelMatrix); 
}

void Model::DrawInstanced(Shader* shader, glm::mat4 cameraMatrix, const std::vector<Transform*>& instanceTransform) {
	for (Mesh& mesh : meshes)
		mesh.DrawInstanced(shader, cameraMatrix, instanceTransform);
}

void Model::DrawInstanced(Shader* shader, glm::mat4 cameraMatrix, DrawableInstanced* drawable) {
	for (Mesh& mesh : meshes)
		mesh.DrawInstanced(shader, cameraMatrix, drawable);
}

void Model::loadModel(std::string path) {
	Assimp::Importer importer;
	// post processing options : force all primitive to be triangles and flip uv on the y axis
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR Assimp " << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('\\'));

	processNode(scene->mRootNode, scene);

	SetAABBMinMax();
}
void Model::processNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		if (mesh->HasNormals()) {
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		if (mesh->mTextureCoords[0]) { //A mesh can contain up to 8 tex coords
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat
	, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		auto itr = texturesLoaded.find(str.C_Str());
		if (itr != texturesLoaded.end()) {
			textures.push_back(itr->second);
		}
		else {
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			texturesLoaded.insert(std::make_pair(str.C_Str(), texture));
		}
	}
	return textures;
}

void Model::SetAABBMinMax() {
	auto itr = meshes.begin();
	if (meshes.end() == itr)
		return;
	auto pair = itr->GetAABBMinMax();
	AABBMin = pair.first;
	AABBMax = pair.second;
	itr++;
	int i = 0;
	for (; itr != meshes.end(); itr++) {
		auto pair = itr->GetAABBMinMax();
		AABBMin.x = std::min(AABBMin.x, pair.first.x);
		AABBMin.y = std::min(AABBMin.y, pair.first.y);
		AABBMin.x = std::min(AABBMin.y, pair.first.z);
		AABBMax.x = std::max(AABBMax.x, pair.second.x);
		AABBMax.y = std::max(AABBMax.y, pair.second.y);
		AABBMax.z = std::max(AABBMax.z, pair.second.z);
	}
}

std::pair<glm::vec3, glm::vec3> Model::GetAABBMinMax() {
	return std::make_pair(AABBMin, AABBMax);
}

/*-------------------Cubemap-------------------*/

Cubemap::Cubemap(Shader* l_shader, std::string dir, std::string ext) 
	:shader(l_shader)
{
	std::vector<std::string> faces
	{
			"right." + ext,
			"left." + ext,
			"up." + ext,
			"down." + ext,
			"front." + ext,
			"back." + ext
	};
	Setup(faces, dir);
}
Cubemap::Cubemap(Shader* l_shader, std::vector<std::string>& faces, std::string dir)
	: shader(l_shader){	
	Setup(faces, dir);
}
Cubemap::~Cubemap(){}

void Cubemap::Setup(std::vector<std::string>& faces, std::string dir) {
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	std::string sep = (*(dir.end() - 1) == '\\') ? "" : "\\";

	glGenTextures(1, &textures);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textures);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load((dir + sep + faces[i]).c_str(),
			&width, &height, &nrChannels, 0);

		GLenum format = GL_RGB;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format,
				width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemapfailedtoloadatpath:" << faces[i]
				<< std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
		GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
		GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
		GL_CLAMP_TO_EDGE);
}

void Cubemap::Draw(glm::mat4 transform) {
	glDepthMask(GL_FALSE);
	shader->use();
	shader->SetUniform("transform", glm::value_ptr(transform));

	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textures);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shader->unuse();
	glDepthMask(GL_TRUE);
}

/*-------------------Helper functions-------------------*/

unsigned int TextureFromFile(const char* name, std::string directory) {
	int width, height, nrChannels;
	unsigned char* data;
	std::string fullPath = directory + "\\" + std::string(name);
	data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);
	
	GLenum format = GL_RGB;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "TextureFromFile : failed to load texture " << fullPath << std::endl;
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}