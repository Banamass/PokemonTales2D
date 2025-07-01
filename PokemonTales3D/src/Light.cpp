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
	win->Draw(sprite);
}