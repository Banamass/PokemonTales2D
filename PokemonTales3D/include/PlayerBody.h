#pragma once

#include "Drawable.h"
#include "Window.h"
#include "EventManager.h"
#include "Map.h"

//Cap the abs value of v by the abs value of cap
void CapFloat(glm::vec3& v, glm::vec3 cap);
//Cap the abs value of vec by the abs value of cap
void CapVec(float& v, float cap);

//Deplet v toward 0 by the value depletion
void DepletFloat(float& v, float depletion);
//Deplet v toward vec(0) by the value depletion
void DepletVec(glm::vec3& v, glm::vec3 depletion);

class PlayerBody {
public:
	PlayerBody(SharedContext* l_context);
	virtual ~PlayerBody();

	void Update(double dt);
	void Render();

	void Accelerate(glm::vec3 f);
	glm::vec3 GetPosition();

private:
	SharedContext* context;

	Drawable sprite;

	float walkSpeed;
	float walkAccel;
	glm::vec3 velocity;
	glm::vec3 maxVelocity;
	glm::vec3 acceleration;
	glm::vec3 velocityDepletionFact;
	glm::vec3 velocityDepletionConst;
};