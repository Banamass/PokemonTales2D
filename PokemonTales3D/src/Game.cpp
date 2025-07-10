#include "Game.h"

Game::Game()
	: window(Constants::WIN_WIDTH, Constants::WIN_HEIGHT, "PokeTales3D", &context), eventManager(&context),
	camera(&context), shaderManager(&context),
	gameSystem(&context),
	light(glm::vec3(10.0f, 50.0f, 10.0f), &context),
	gui(&context),
	font("Resources\\fonts\\arial.ttf"),
	text(&font, "Bonjour")
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

	text.SetPos(glm::vec2(0, 400));
	text.SetCharacterSize(10);
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

	window.DrawStatic(&text);

	window.EndDraw();
}

bool Game::IsOver() {
	return window.GetShouldClose();
}