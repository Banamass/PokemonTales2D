#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H //See https://learnopengl.com/In-Practice/Text-Rendering

#include "Shader.h"
#include "SharedTypes.h"
#include "Drawable.h"
#include "ResourceManager.h"

//Structure storing all informations about a loaded character
struct Character {
	unsigned int textureID; //Opengl context id
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

//This class represent a font, which can load a character of a given font
class Font {
public:
	Font(std::string l_fontFile, int l_glyphSize = 20);
	~Font();

	//Get a pointer to constant character object matching with the char in arg
	const Character* GetCharacter(char c);

	//Get the size of the loaded font glyph
	int GetGlyphSize();

private:
	int glyphSize;

	std::unordered_map<char, Character> characters;

	FT_Library ft;
	FT_Face face;
};

class FontManager : public ResourceManager<FontManager, Font> {
public:
	FontManager(SharedContext* context) : ResourceManager("Resources\\fonts\\fonts.cfg") {
		context->fontManager = this;
	}

	Font* Load(const std::vector<std::string>* l_args)
	{
		if (l_args->size() < 1)
			return nullptr;
		Font* res = nullptr;
		if (l_args->size() == 1) {
			res = new Font(l_args->at(0).data());
		}
		else {
			try {
				res = new Font(l_args->at(0).data(), std::stoi(l_args->at(1)));
			}
			catch (std::invalid_argument e) {
				std::cout << "In fontManager load method : " << e.what() << std::endl;
				res = new Font(l_args->at(0).data());
			}
		}
		return res;
	}
};

//This class is a drawable static which represents a text
class Text : public DrawableStatic {
public:
	Text(Font* l_font, std::string l_text, Shader* l_shader);
	virtual ~Text();

	virtual void Draw(glm::mat4& cameraMatrix);

	//Set the whole text string
	void SetText(std::string l_text);
	//Append a text to the current one
	void AddText(std::string l_textAdded);
	//Remove nbCh from the text
	void RemoveText(unsigned int nbCh);

	//Set the character size of the text, in pixel
	void SetCharacterSize(float l_size);
	//Set the color of the text
	void SetColor(glm::vec3 l_color);

	//Get the string represented by the text
	std::string GetText();

	virtual FloatRect GetFloatRect();

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
	float scale;
	glm::vec3 color;
	FloatRect floatRect;

	std::vector<CharacterData> textData;
};