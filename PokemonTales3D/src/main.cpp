#include "Game.h"

int main() {
	Game game;

	double lastTime = glfwGetTime();
	while (!game.IsOver()) {
		double currTime = glfwGetTime();
		double dt = lastTime - currTime;
		lastTime = currTime;
		game.Update(dt);
		game.Render();
	}

	return 0;
}