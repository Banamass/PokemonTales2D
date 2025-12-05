#include "Light.h"

Light::Light(glm::vec3 l_pos, SharedContext* l_context)
	: shader(l_context->shaderManager->GetShader("SimpleShader")), context(l_context), pos(l_pos)
{
	if (!l_context->modelManager->RequireResource("Cube")) {
		std::cout << "Cube model not found" << std::endl;
	}
	model = l_context->modelManager->GetResource("Cube");

	sprite = new Drawable(model, shader);
	sprite->Move(pos);
}
Light::~Light() {
	delete sprite;
	context->modelManager->ReleaseResource("Cube");
}

void Light::Draw() {
	shader->use();
	shader->SetUniform("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	context->win->Draw(sprite);
}

void Light::Activate() {
	context->shaderManager->SetLightPos(pos);
}
void Light::Desactivate() {

}