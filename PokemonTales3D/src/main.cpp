#include "Game.h"

int main() {
	Game game;

	double lastTime = glfwGetTime();
	while (!game.IsOver()) {
		double currTime = glfwGetTime();
		double dt = currTime - lastTime;
		if (dt > 0.05f)   // 50ms max
			dt = 0.05f;
		lastTime = currTime;
		game.Update(dt);
		game.Render();
	}

	return 0;
}