#include "Camera.h"

Camera::Camera(SharedContext* l_context, StateType state)
	:window(context->win), context(l_context), isFollowingMouse(true) {
	glm::vec2 l_windowSize = window->GetWindowSize();

	pos = glm::vec3(-3.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	SetFront(glm::vec3(1.0f, 0.0f, 0.0f));

	view = glm::lookAt(pos, pos + front, up);

	SetFoV(45.0f);

	transformMatrix = projection * view;

	lastMousPos = l_windowSize / 2.0f;
	firstMouse = true;
	pitch = 0.0f;
	yaw = 0.0f;
	sensitivity = 0.1f;

	zoomSensitivity = 1.0f;

	context->eventManager->AddCallback("CameraMove", EventType::MouseMove, &Camera::MouseMouseCallback, this, state);
	context->eventManager->AddCallback("CameraScroll", EventType::Scroll, &Camera::ScrollCallback, this, state);

	context->camera = this;
}
Camera::~Camera(){
	context->eventManager->RemoveCallbacks("CameraMove");
	context->eventManager->RemoveCallbacks("CameraScroll");
}

void Camera::Update(const double& dt) {
	glm::vec3 move = glm::vec3(0.0f, 0.0f, 0.0f);

	float speed = 2.0f;

	if (glfwGetKey(window->GetGLFWwindow(), config.forwardK) == GLFW_PRESS)
		move -= (float)dt * (speed * frontMove);
	if (glfwGetKey(window->GetGLFWwindow(), config.backwardK) == GLFW_PRESS)
		move += (float)dt * (speed * frontMove);
	if (glfwGetKey(window->GetGLFWwindow(), config.leftK) == GLFW_PRESS)
		move += (float)dt * (speed * rightMove);
	if (glfwGetKey(window->GetGLFWwindow(), config.rightK) == GLFW_PRESS)
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

	upDir = -glm::cross(rightMove, front);
}

void Camera::SetPosition(glm::vec3 newPos) {
	pos = newPos;

	view = glm::lookAt(pos, pos + front, up);

	transformMatrix = projection * view;
}

void Camera::SetFoV(float newFoV) {
	FoV = newFoV;
	FovY = glm::radians(FoV);
	float aspect = Constants::WIN_WIDTH / Constants::WIN_HEIGHT;
	FovX = atan(tan(FovY / 2) * aspect) * 2;

	glm::vec2 l_windowSize = window->GetWindowSize();
	projection = glm::perspective(glm::radians(FoV), l_windowSize.x / l_windowSize.y, 0.1f, 100.0f);

	transformMatrix = projection * view;
}

void Camera::SetIsFollowingMouse(float b) {
	isFollowingMouse = b;
	context->win->SetCursorCapture(b);
	firstMouse = true;
}

void Camera::SetKeyConfig(KeyConfig l_config) {
	config = l_config;
}

void Camera::MouseMouseCallback(CallbackData data) {
	if (!isFollowingMouse)
		return;
	
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
	/*Scroll_Data* mdata = std::get_if<Scroll_Data>(&data.data);
	if (mdata == nullptr)
		return;

	float newFoV = FoV - zoomSensitivity * mdata->yoffset;
	newFoV = std::max(1.0f, newFoV);
	newFoV = std::min(170.0f, newFoV);
	SetFoV(newFoV);*/
}

const glm::mat4& Camera::GetTransformMatrix(){
	return transformMatrix;
}

glm::mat4 Camera::GetSkyboxTransformMatrix() {
	return projection * glm::mat4(glm::mat3(view));
}

glm::vec3 Camera::GetMouseDirection() {
	glm::vec3 mouseDir = glm::normalize(front);
	if (isFollowingMouse) {
		return mouseDir;	
	}
	glm::vec2 mousePos = window->GetMousePos();

	glm::vec2 screenMiddle(Constants::WIN_WIDTH / 2.0f, Constants::WIN_HEIGHT / 2.0f);
	glm::vec2 delta = mousePos - screenMiddle;	

	glm::vec2 dAlpha = delta / screenMiddle * glm::vec2(FovX/2.0f, FovY/2.0f);

	glm::mat4 mat(1.0f);
	mat = glm::rotate(mat, dAlpha.y, rightMove);
	mat = glm::rotate(mat, dAlpha.x, upDir);
	mouseDir = glm::vec3(mat * glm::vec4(mouseDir, 1.0f));

	return glm::normalize(mouseDir);
}

glm::vec3 Camera::GetPosition() {
	return pos;
}