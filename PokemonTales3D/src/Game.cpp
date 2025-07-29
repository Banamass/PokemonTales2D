#include "Game.h"

Game::Game()
	: window(Constants::WIN_WIDTH, Constants::WIN_HEIGHT, "PokeTales3D", &context), eventManager(&context),
	camera(&context),
	shaderManager(&context), modelManager(&context), fontManager(&context),
	gameSystem(&context),
	light(glm::vec3(-70.0f, 50.0f, 10.0f), &context),
	gui(&context)
{
	window.SetCursorCapture(true);
	camera.SetPosition(glm::vec3(-3.0f, 5.0f, -3.0f));

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
}
Game::~Game(){
	delete skybox;
}

void Game::Update(double dt){
	camera.Update(dt);
	gameSystem.Update(dt);
	gui.Update(dt);
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