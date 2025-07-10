#include "Game.h"

Game::Game() 
	: window(800, 600, "PokeTales3D", &context), eventManager(&context),
	camera(&context), shaderManager(&context),
	gameSystem(&context),
	light(glm::vec3(10.0f, 50.0f, 10.0f), &context),
	gui(&context)
{
	window.SetCursorCapture(true);

	std::string dir = "Resources\\Skybox";
	std::vector<std::string> faces
	{
			"right.jpg",
			"left.jpg",
			"top.jpg",
			"bottom.jpg",
			"front.jpg",
			"back.jpg"
	};
	skybox = new Cubemap(shaderManager.GetShader("CubemapShader"), faces, dir);

	InitFont();
}
Game::~Game(){}

void Game::Update(double dt){
	camera.Update(dt);
	gameSystem.Update(dt);
}
void Game::Render(){
	window.StartDraw();

	window.Draw(skybox);

	gameSystem.Render();
	gui.Render();
	light.Draw(&window);

	window.EndDraw();
}

bool Game::IsOver() {
	return window.GetShouldClose();
}

void Game::InitFont() {
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "Error FREETYPE : could not init free type library" << std::endl;
		return;
	}

	FT_Face face;
	if (FT_New_Face(ft, "Resources\\fonts\\arial.ttf", 0, &face)) {
		std::cout << "Error FREETYPE : failed to load font" << std::endl;
		return;
	}
}