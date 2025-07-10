#pragma once

#include "Window.h"
#include "SharedTypes.h"

class GUI {
public:
	GUI(SharedContext* l_context);

	void Render();

private:
	SharedContext* context;

	Model cursorModel;
	Drawable cursor;
};