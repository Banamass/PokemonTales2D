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
	Text(Font* l_font, std::string l_text);
	~Text();

	void Draw(Shader* l_shader);

	void SetPos(glm::vec2 l_pos);
	void SetCharacterSize(float l_size);
	void SetColor(glm::vec3 l_color);

private:
	struct CharacterData {
		CharacterData(const Character* l_cInfos) 
			: cInfos(l_cInfos), vertices(){}
		float vertices[6][4];
		const Character* cInfos;
	};

	void ComputeCharacterDatas();

	unsigned int VAO, VBO;

	Font* font;

	std::string text;
	glm::vec2 pos;
	float scale;
	glm::vec3 color;

	std::vector<CharacterData> textData;
};