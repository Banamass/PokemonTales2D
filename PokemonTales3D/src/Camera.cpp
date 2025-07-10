#include "Camera.h"

Camera::Camera(SharedContext* l_context)
	:window(context->win), context(l_context) {
	glm::vec2 l_windowSize = window->GetWindowSize();

	pos = glm::vec3(-3.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	SetFront(glm::vec3(1.0f, 0.0f, 0.0f));

	view = glm::lookAt(pos, pos + front, up);

	FoV = 45.0;
	projection = glm::perspective(glm::radians(45.0f), l_windowSize.x / l_windowSize.y, 0.1f, 100.0f);

	transformMatrix = projection * view;

	lastMousPos = l_windowSize / 2.0f;
	firstMouse = true;
	pitch = 0.0f;
	yaw = 0.0f;
	sensitivity = 0.1f;

	zoomSensitivity = 1.0f;

	context->eventManager->AddCallback(EventType::MouseMove, &Camera::MouseMouseCallback, this);
	context->eventManager->AddCallback(EventType::Scroll, &Camera::ScrollCallback, this);

	context->camera = this;
}
Camera::~Camera(){
	
}

void Camera::Update(const double& dt) {
	glm::vec3 move = glm::vec3(0.0f, 0.0f, 0.0f);

	float speed = 2.0f;

	if (glfwGetKey(window->GetGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS)
		move -= (float)dt * (speed * frontMove);
	if (glfwGetKey(window->GetGLFWwindow(), GLFW_KEY_S) == GLFW_PRESS)
		move += (float)dt * (speed * frontMove);
	if (glfwGetKey(window->GetGLFWwindow(), GLFW_KEY_A) == GLFW_PRESS)
		move += (float)dt * (speed * rightMove);
	if (glfwGetKey(window->GetGLFWwindow(), GLFW_KEY_D) == GLFW_PRESS)
		move -= (float)dt * (speed * rightMove);

	if (glfwGetKey(window->GetGLFWwindow(), GLFW_KEY_UP) == GLFW_PRESS)
		move.y -= (float)dt * speed;
	if (glfwGetKey(window->GetGLFWwindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
		move.y += (float)dt * speed;

	SetPosition(pos + move);

	context->shaderManager->SetViewPos(pos);
}

void Camera::SetFront(glm::vec3 newFront) {
	front = newFront;

	view = glm::lookAt(pos, pos + front, up);

	transformMatrix = projection * view;

	frontMove = glm::vec3(front.x, 0.0f, front.z);
	frontMove = glm::normalize(frontMove);
	rightMove = glm::cross(frontMove, up);
}

void Camera::SetPosition(glm::vec3 newPos) {
	pos = newPos;

	view = glm::lookAt(pos, pos + front, up);

	transformMatrix = projection * view;
}

void Camera::SetFoV(float newFoV) {
	FoV = newFoV;

	glm::vec2 l_windowSize = window->GetWindowSize();
	projection = glm::perspective(glm::radians(FoV), l_windowSize.x / l_windowSize.y, 0.1f, 100.0f);

	transformMatrix = projection * view;
}

void Camera::MouseMouseCallback(CallbackData data) {
	MouseMove_Data* mdata = std::get_if<MouseMove_Data>(&data.data);
	if (mdata == nullptr)
		return;
	if (firstMouse) {
		lastMousPos = glm::vec2(mdata->xpos, mdata->ypos);
		firstMouse = false;
		return;
	}

	float xoffset = mdata->xpos - lastMousPos.x;
	float yoffset = lastMousPos.y - mdata->ypos;
	lastMousPos = glm::vec2(mdata->xpos, mdata->ypos);

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;
	pitch = std::max(pitch, -89.0f);
	pitch = std::min(pitch, 89.0f);

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	SetFront(glm::normalize(direction));
}

void Camera::ScrollCallback(CallbackData data) {
	Scroll_Data* mdata = std::get_if<Scroll_Data>(&data.data);
	if (mdata == nullptr)
		return;

	float newFoV = FoV - zoomSensitivity * mdata->yoffset;
	newFoV = std::max(1.0f, newFoV);
	newFoV = std::min(170.0f, newFoV);
	SetFoV(newFoV);
}

const glm::mat4& Camera::GetTransformMatrix(){
	return transformMatrix;
}

glm::mat4 Camera::GetSkyboxTransformMatrix() {
	return projection * glm::mat4(glm::mat3(view));
}

glm::vec3 Camera::GetMouseDirection() {
	return glm::normalize(front);
}

glm::vec3 Camera::GetPosition() {
	return pos;
}