#pragma once

#include "EventManager.h"
#include "Window.h"

class Camera {
public:
	Camera(EventManager* eventManager, Window* l_window);
	~Camera();

	void Update(const double& dt);

	const glm::mat4& GetTransformMatrix();

	void MouseMouseCallback(CallbackData data);
	void ScrollCallback(CallbackData data);

	void SetFront(glm::vec3 newFront);
	void SetPosition(glm::vec3 newPos);
	void SetFoV(float newFoV);

private:
	Window* window;

	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 transformMatrix;

	glm::vec3 pos; //position of the camera
	glm::vec3 front; //the front direction of the camera
	glm::vec3 up; //the upward direction of the camera

	glm::vec3 frontMove; //the front direction of the camera
	glm::vec3 rightMove; //the right direction of the camera

	bool firstMouse;
	glm::vec2 lastMousPos;

	float yaw;
	float pitch;
	float sensitivity;

	float FoV;
	float zoomSensitivity;
};