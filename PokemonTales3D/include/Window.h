#pragma once

#include "Drawable.h"
#include "Font.h"
#include "Shape.h"

/* Class reprensenting the window
It's an interface between glfw and OpenGL, and the rest of the code*/
class Window {
public:
	Window(int l_width, int l_height, const std::string& winName, SharedContext* context);
	~Window();

	/*Begin the graphic pipelines*/
	void StartDraw();
	/*End the graphic pipelines*/
	void EndDraw();

	/*Drawing methods for 3D object, in world coordinates*/

	void Draw(Drawable& drawable);
	void Draw(Drawable* drawable);
	void Draw(Cubemap& cubemap);
	void Draw(Cubemap* cubemap);

	/* Drawing methods for the 2D or 3D objects, in screen coordinates */

	void DrawStatic(Drawable* drawable);
	void DrawStatic(Text* text);
	void DrawStatic(RectangleShape* shape);
	void DrawStatic(DrawableStatic* drawable);

	/* Drawing methods for instanced objects, in world coordinates */

	void DrawInstanced(DrawableInstanced* drawableInstanced);

	/* Draw objects but add some outline, in world coordinates*/

	void DrawOutlined(Drawable& drawable, glm::vec4 color, float thickness);

	//Set if the cursor is capture by the window or not
	void SetCursorCapture(bool capture);

	//Get the size of the window
	glm::vec2 GetWindowSize();
	//Get if the window should be close
	bool GetShouldClose();
	//Get the glfw window object associated to that window interface
	GLFWwindow* GetGLFWwindow();
	//Get the mouse pos, with a top left origin
	glm::vec2 GetMousePos();

	//Close the window
	void Close();

private:
	void ProcessInput();

	SharedContext* context;

	GLFWwindow* window;
	int width;
	int height;
	glm::mat4 projectionMat;
};