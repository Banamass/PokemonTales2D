#include "GUI.h"

GUI::GUI(SharedContext* l_context)
	: context(l_context), cursorModel("Resources\\Box\\box.obj"),
	cursor(&cursorModel, l_context->shaderManager->GetShader("SimpleShader")) {
	cursor.Scale(glm::vec3(0.03f, 0.25f, 0.03f));
}

void GUI::Render() {
	context->win->DrawStatic(&cursor);
}