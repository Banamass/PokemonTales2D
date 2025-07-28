#include "Font.h"

/*----------------Font-------------------*/

Font::Font(std::string l_fontFile, int l_glyphSize) 
	: glyphSize(l_glyphSize){
	ft = 0;
	face = 0;

	if (FT_Init_FreeType(&ft)) {
		std::cout << "Error FREETYPE : could not init free type library" << std::endl;
		return;
	}

	if (FT_New_Face(ft, l_fontFile.c_str(), 0, &face)) {
		std::cout << "Error FREETYPE : failed to load font" << std::endl;
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, glyphSize);
}
Font::~Font(){
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

const Character* Font::GetCharacter(char c) {
	auto itr = characters.find(c);
	if (itr != characters.end())
		return &itr->second;

	if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
		return nullptr;
	}
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //no byte-aligment restriction
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		face->glyph->bitmap.width,
		face->glyph->bitmap.rows,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		face->glyph->bitmap.buffer
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Character character = {
		texture,
		glm::ivec2(face->glyph->bitmap.width,face->glyph->bitmap.rows),
		glm::ivec2(face->glyph->bitmap_left,face->glyph->bitmap_top),
		face->glyph->advance.x
	};
	characters.insert(std::pair<char, Character>(c, character));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	return &characters[c];
}

int Font::GetGlyphSize() { return glyphSize; }

/*----------------Text-------------------*/

Text::Text(Font* l_font, std::string l_text, Shader* l_shader) 
	:DrawableStatic(), font(l_font), shader(l_shader) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	pos = glm::vec2(0, 0);
	scale = 1.0f;
	color = glm::vec3(1.0f, 1.0f, 1.0f);

	SetText(l_text);
}
Text::~Text() {}

void Text::ComputeCharacterDatas(int l_offset) {
	if (textData.size() == 0)
		return;

	glm::vec2 realPos = offset + pos;

	float x = realPos.x;
	float minypos = Constants::WIN_HEIGHT * 2;
	float maxh = -1;

	auto cData = textData.begin();
	for (; cData != textData.begin() + l_offset && cData != textData.end(); cData++) {
		unsigned int advance = cData->cInfos->advance;
		x += (advance >> 6) * scale; //bitshift by 6
		float ypos = realPos.y - (cData->cInfos->size.y - cData->cInfos->bearing.y) * scale;
		minypos = std::min(ypos, minypos);
	}
	for (; cData != textData.end(); cData++) {
		float xpos = x + cData->cInfos->bearing.x * scale;
		float ypos = realPos.y - (cData->cInfos->size.y - cData->cInfos->bearing.y) * scale;

		float w = cData->cInfos->size.x * scale;
		float h = cData->cInfos->size.y * scale;

		float vertices[6][4] = {
			{xpos, ypos + h, 0.0f,0.0f},
			{xpos, ypos, 0.0f,1.0f},
			{xpos + w,ypos, 1.0f,1.0f},
			{xpos, ypos + h, 0.0f,0.0f},
			{xpos + w,ypos, 1.0f,1.0f},
			{xpos + w,ypos + h, 1.0f,0.0f}
		};
		for (int i = 0; i < 6; i++)
			for (int j = 0; j < 4; j++)
				cData->vertices[i][j] = vertices[i][j];

		unsigned int advance = cData->cInfos->advance;
		x += (advance >> 6) * scale; //bitshift by 6

		minypos = std::min(ypos, minypos);
		maxh = std::max(h, maxh);
	}
	hitbox.size.x = x - realPos.x;
	hitbox.size.y = std::max(maxh, hitbox.size.y);
	hitbox.pos.x = realPos.x;
	hitbox.pos.y = minypos;
}

void Text::Draw(glm::mat4& cameraMatrix) {
	shader->use();
	shader->SetUniform("projection", glm::value_ptr(cameraMatrix));
	shader->SetUniform("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	float x = pos.x;

	for (auto& c : textData) {
		glBindTexture(GL_TEXTURE_2D, c.cInfos->textureID);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(c.vertices), c.vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glBindVertexArray(0);
}

void Text::SetText(std::string l_text) {
	text = l_text;
	textData.clear();
	for (char c : l_text) {
		const Character* cInfos = font->GetCharacter(c);
		if (cInfos == nullptr)
			std::cout << "error c isn't supported by the given font" << std::endl;
		textData.emplace_back(cInfos);
		(textData.end() - 1)->cInfos = cInfos;
	}

	ComputeCharacterDatas(0);
}
void Text::AddText(std::string l_textAdded) {
	int off = text.size();
	text = text + l_textAdded;
	for (char c : l_textAdded) {
		const Character* cInfos = font->GetCharacter(c);
		if (cInfos == nullptr)
			std::cout << "error c isn't supported by the given font" << std::endl;
		textData.emplace_back(cInfos);
		(textData.end() - 1)->cInfos = cInfos;
	}

	ComputeCharacterDatas(off);
}

void Text::RemoveText(unsigned int nbCh) {
	for (int i = 0; i < nbCh && !text.empty(); i++) {
		textData.pop_back();
		text.pop_back();
	}
	float minypos = Constants::WIN_HEIGHT * 2;
	float maxh = -1;
	float x = 0;

	auto cData = textData.begin();
	for (; cData != textData.end(); cData++) {
		unsigned int advance = cData->cInfos->advance;
		x += (advance >> 6) * scale; //bitshift by 6
		float ypos = pos.y - (cData->cInfos->size.y - cData->cInfos->bearing.y) * scale;
		minypos = std::min(ypos, minypos);
		float h = cData->cInfos->size.y * scale;
		maxh = std::max(h, maxh);
	}

	hitbox.size.x = x - pos.x;
	hitbox.size.y = maxh;
	hitbox.pos.y = minypos;
}

FloatRect Text::GetFloatRect() { return hitbox; }
std::string Text::GetText() { return text; }

void Text::SetPos(glm::vec2 l_pos) { 
	if (pos == l_pos)
		return;
	pos = l_pos;
	ComputeCharacterDatas(0); 
}
void Text::SetOffset(glm::vec2 l_offset) {
	if (offset == l_offset)
		return;
	offset = l_offset;
	ComputeCharacterDatas(0);
}
void Text::SetCharacterSize(float l_size) { scale = l_size / font->GetGlyphSize(); ComputeCharacterDatas(0); }
void Text::SetColor(glm::vec3 l_color) { color = l_color; }