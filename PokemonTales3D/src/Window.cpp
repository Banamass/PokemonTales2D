#include "Window.h"
#include "Camera.h"

/*------------------------Window------------------------*/

Window::Window(int l_width, int l_height, const std::string& winName, SharedContext* l_context)
	: width(l_width), height(l_height), context(l_context){
	glewExperimental = true;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(width, height, winName.c_str(), NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	glViewport(0, 0, width, height);

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	context->win = this;
}
Window::~Window() {
	glfwTerminate();
}

void Window::ProcessInput() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Window::StartDraw() {
	ProcessInput();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
void Window::EndDraw() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::Draw(Drawable& drawable) {
	Draw(&drawable);
}

void Window::Draw(Drawable* drawable) {
	Shader* shader = drawable->shader;
	shader->use();
	shader->SetUniform("material.ambient", drawable->material.ambient);
	shader->SetUniform("material.diffuse", drawable->material.diffuse);
	shader->SetUniform("material.specular", drawable->material.specular);
	shader->SetUniform("material.shininess", drawable->material.shininess);
	drawable->model->Draw(shader, context->camera->GetTransformMatrix(), drawable->transform.GetTransform());
}

void Window::Draw(Cubemap& cubemap) {
	Draw(&cubemap);
}
void Window::Draw(Cubemap* cubemap) {
	cubemap->Draw(context->camera->GetSkyboxTransformMatrix());
}

void Window::DrawStatic(Drawable* drawable) {
	Shader* shader = drawable->shader;
	shader->use();
	shader->SetUniform("material.ambient", drawable->material.ambient);
	shader->SetUniform("material.diffuse", drawable->material.diffuse);
	shader->SetUniform("material.specular", drawable->material.specular);
	shader->SetUniform("material.shininess", drawable->material.shininess);
	drawable->model->Draw(shader, glm::mat4(1.0f), drawable->transform.GetTransform());
}

void Window::DrawStatic(Text* text) {
	text->Draw();
}

void Window::DrawStatic(Shape* shape) {
	shape->Draw();
}

void Window::DrawInstanced(DrawableInstanced* drawableInstanced) {
	Drawable* drawable = drawableInstanced->drawable;
	Shader* shader = drawable->shader;
	shader->use();
	shader->SetUniform("material.ambient", drawable->material.ambient);
	shader->SetUniform("material.diffuse", drawable->material.diffuse);
	shader->SetUniform("material.specular", drawable->material.specular);
	shader->SetUniform("material.shininess", drawable->material.shininess);
	drawable->model->DrawInstanced(shader, context->camera->GetTransformMatrix(),
		drawableInstanced);
}

void Window::DrawOutlined(Drawable& drawable, glm::vec4 color, float thickness) {
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	Draw(drawable);

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	Shader* simpleShader = context->shaderManager->GetShader("SimpleShader");
	simpleShader->use();
	simpleShader->SetUniform("color", color);
	glm::vec3 scaling = (1.0f + thickness) * glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 tempScaling = drawable.transform.scaling;
	glm::mat4 tempTransform = drawable.transform.transform;
	drawable.Scale(scaling);
	drawable.model->Draw(simpleShader, context->camera->GetTransformMatrix(), drawable.transform.transform);
	drawable.transform.scaling = tempScaling;
	drawable.transform.transform = tempTransform;
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glEnable(GL_DEPTH_TEST);
}

void Window::SetCursorCapture(bool capture) {
	if (capture) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

bool Window::GetShouldClose() {
	return glfwWindowShouldClose(window);
}

glm::vec2 Window::GetWindowSize() {
	return glm::vec2(width, height);
}
GLFWwindow* Window::GetGLFWwindow() {
	return window;
}
