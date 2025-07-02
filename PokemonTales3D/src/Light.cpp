#include "Light.h"

Light::Light(glm::vec3 pos, SharedContext* l_context)
	: model("Resources\\Box\\cube.obj"),
	shader(l_context->shaderManager->GetShader("SimpleShader")),
	sprite(&model, shader){
	sprite.Move(pos);
	l_context->shaderManager->SetLightPos(pos);
}
Light::~Light() {
	
}

void Light::Draw(Window* win) {
	shader->use();
	shader->SetUniform("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	win->Draw(sprite);
}