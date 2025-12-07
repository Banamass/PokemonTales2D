#pragma once

#include "EventManager.h"

class Camera {
public:
	struct KeyConfig {
		KeyConfig()
			: forwardK(AZERTY::Z), backwardK(AZERTY::S), leftK(AZERTY::Q), rightK(AZERTY::D),
			upK(AZERTY::ARROW_UP), downK(AZERTY::ARROW_DOWN) {
		}

		int forwardK;
		int backwardK;
		int leftK;
		int rightK;

		int upK;
		int downK;
	};

	Camera(SharedContext* l_context, StateType state);
	~Camera();

	//Update the camera state
	void Update(const double& dt);

	//Get the transform matrix associated to the view and the projection for 3D world object rendering 
	const glm::mat4& GetTransformMatrix();
	//Get the transform matrix associated to the view and the projection for 3D world cubemap rendering 
	glm::mat4 GetSkyboxTransformMatrix();

	void MouseMouseCallback(CallbackData data);
	void ScrollCallback(CallbackData data);

	//Set the front vector of the camera
	void SetFront(glm::vec3 newFront);
	//Set the position of the camera
	void SetPosition(glm::vec3 newPos);
	//Set the field of view of the camera (in degree)
	void SetFoV(float newFoV);
	//Set if the camerais following the mouse pos
	void SetIsFollowingMouse(float b);
	//Set the key config of the camera
	void SetKeyConfig(KeyConfig l_config);

	//Getting the direction where the mouse is currently pointing to
	glm::vec3 GetMouseDirection();
	//Get the position of the camera
	glm::vec3 GetPosition();

	

private:
	SharedContext* context;
	Window* window;

	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 transformMatrix;

	glm::vec3 pos; //position of the camera
	glm::vec3 front; //the front direction of the camera
	glm::vec3 up; //the upward direction of the world
	glm::vec3 upDir; //the upward direction of the camera

	glm::vec3 frontMove; //the front move of the camera
	glm::vec3 rightMove; //the right move of the camera
	

	bool firstMouse;
	glm::vec2 lastMousPos;
	bool isFollowingMouse;

	float yaw;
	float pitch;
	float sensitivity;

	float FoV;
	float FovX;
	float FovY;
	float zoomSensitivity;

	KeyConfig config;
};