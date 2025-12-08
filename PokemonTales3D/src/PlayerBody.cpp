#include "PlayerBody.h"

void CapFloat(float& v, float cap) {
	if (v >= 0.0f)
		v = (v > cap ? cap : v);
	else
		v = (v < -cap ? -cap : v);
}
void CapVec(glm::vec3& vec, glm::vec3 cap) {
	CapFloat(vec.x, cap.x);
	CapFloat(vec.y, cap.y);
	CapFloat(vec.z, cap.z);
}

void DepletFloat(float& v, float depletion) {
	if (v >= 0.0f)
		v = std::max(0.0f, v - depletion);
	else
		v = std::min(0.0f, v + depletion);
}
void DepletVec(glm::vec3& v, glm::vec3 depletion) {
	DepletFloat(v.x, depletion.x);
	DepletFloat(v.y, depletion.y);
	DepletFloat(v.z, depletion.z);
}

PlayerBody::PlayerBody(SharedContext* l_context)
	: context(l_context), walkSpeed(6.0f), walkAccel(26.0f), velocity(0.0f), acceleration(0.0f),
	maxVelocity(walkSpeed, 10.0f, walkSpeed), velocityDepletionFact(3.2f), velocityDepletionConst(6.0f)
{
	sprite.SetModel(context->modelManager->RequireGetResource("Character"));
	sprite.SetShader(context->shaderManager->GetShader("ModelShader"));

	Drawable::Material mat;
	mat.SetLightningColor(glm::vec3(1.0f), 0.7f, 0.0f);
	sprite.SetMaterial(mat);

	sprite.Move(glm::vec3(2.0f, 0.0f, 0.0f));

	SetFrontMove(glm::vec3(1, 0, 0));
}
PlayerBody::~PlayerBody(){
	context->modelManager->ReleaseResource("Character");
}

void PlayerBody::Update(double dt){
	GLFWwindow* gwin = context->win->GetGLFWwindow();

	if (glfwGetKey(gwin, AZERTY::Z) == GLFW_PRESS) {
		acceleration += frontMove;
	}
	if (glfwGetKey(gwin, AZERTY::S) == GLFW_PRESS) {
		acceleration -= frontMove;
	}
	if (glfwGetKey(gwin, AZERTY::Q) == GLFW_PRESS) {
		acceleration -= rightMove;
	}
	if (glfwGetKey(gwin, AZERTY::D) == GLFW_PRESS) {
		acceleration += rightMove;
	}

	velocity += acceleration * (float)dt;
	acceleration = glm::vec3(0.0f);
	CapVec(velocity, maxVelocity);

	glm::vec3 prevPos = sprite.GetPosition();
	glm::vec3 newPos = sprite.GetPosition();

	newPos += (float)dt * velocity;

	//std::cout << prevPos << " \n" << newPos << std::endl;
	glm::vec3 finalPos = context->map->ReactMovment(prevPos, newPos);
	//std::cout << finalPos << std::endl;
	bool change = false;
	if (finalPos.x != prevPos.x) {
		change = true;
	}
	else {
		velocity.x = 0.0f;
	}
	if (finalPos.y != prevPos.y) {
		change = true;
	}
	else {
		velocity.y = 0.0f;
		acceleration.y += 30.0f;
	}
	if (finalPos.z != prevPos.z) {
		change = true;
	}
	else {
		velocity.z = 0.0f;
	}
	if (change) {
		sprite.SetPosition(finalPos);
	}
	glm::vec3 absVel = glm::vec3(std::abs(velocity.x), std::abs(velocity.y), std::abs(velocity.z));
	glm::vec3 depletion = (velocityDepletionFact * absVel + velocityDepletionConst) * (float)dt;
	depletion.y = 0.0f;
	DepletVec(velocity, depletion);
}
void PlayerBody::Render(){
	context->win->Draw(sprite);
}

void PlayerBody::SetFrontMove(glm::vec3 v) {
	frontMove = glm::normalize(v);
	rightMove = glm::cross(frontMove, glm::vec3(0.0f, 1.0f, 0.0f));
	frontMove *= walkAccel;
	rightMove *= walkAccel;
}

void PlayerBody::Accelerate(glm::vec3 f) {
	acceleration += f;
}

glm::vec3 PlayerBody::GetPosition() {
	return sprite.GetPosition();
}