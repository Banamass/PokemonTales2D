#pragma once

#include "Drawable.h"
#include "Font.h"
#include "Shape.h"

class Window {
public:
	Window(int l_width, int l_height, const std::string& winName, SharedContext* context);
	~Window();

	void StartDraw();
	void EndDraw();

	void Draw(Drawable& drawable);
	void Draw(Drawable* drawable);
	void Draw(Cubemap& cubemap);
	void Draw(Cubemap* cubemap);

	void DrawStatic(Drawable* drawable);
	void DrawStatic(Text* text);
	void DrawStatic(Shape* shape);
	void DrawInstanced(DrawableInstanced* drawableInstanced);
	void DrawOutlined(Drawable& drawable, glm::vec4 color, float thickness);
	void SetCursorCapture(bool capture);

	glm::vec2 GetWindowSize();
	bool GetShouldClose();
	GLFWwindow* GetGLFWwindow();

private:
	void ProcessInput();

	SharedContext* context;

	GLFWwindow* window;
	int width;
	int height;
};