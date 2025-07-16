#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"
#include "SharedTypes.h"

struct Character {
	unsigned int textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

class Font {
public:
	Font(std::string l_fontFile, int l_glyphSize = 20);
	~Font();

	const Character* GetCharacter(char c);

	int GetGlyphSize();

private:
	int glyphSize;

	std::unordered_map<char, Character> characters;

	FT_Library ft;
	FT_Face face;
};

class Text {
public:
	Text(Font* l_font, std::string l_text, Shader* l_shader);
	~Text();

	void Draw();

	void SetText(std::string l_text);
	void AddText(std::string l_textAdded);
	void RemoveText(unsigned int nbCh);

	void SetPos(glm::vec2 l_pos);
	void SetCharacterSize(float l_size);
	void SetColor(glm::vec3 l_color);

	FloatRect GetFloatRect();
	glm::vec2 GetPos();
	std::string GetText();

private:
	struct CharacterData {
		CharacterData(const Character* l_cInfos) 
			: cInfos(l_cInfos), vertices(){}
		float vertices[6][4];
		const Character* cInfos;
	};

	//Compute the data of character data textData[offset, .., size[
	void ComputeCharacterDatas(int offset);

	Shader* shader;
	unsigned int VAO, VBO;

	Font* font;

	std::string text;
	glm::vec2 pos;
	float scale;
	glm::vec3 color;
	FloatRect hitbox;

	std::vector<CharacterData> textData;
};