#include "EventManager.h"

EventManager::EventManager(SharedContext* l_context)
	: window(l_context->win->GetGLFWwindow()), context(l_context){
	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPosCallback(window, EventManager::_MouseMoveCallback);
	glfwSetScrollCallback(window, EventManager::_ScrollCallback);
	glfwSetKeyCallback(window, EventManager::_KeyCallback);
	glfwSetMouseButtonCallback(window, EventManager::_MouseButtonCallback);

	context->eventManager = this;
}
EventManager::~EventManager(){}

void EventManager::_KeyCallback(GLFWwindow* l_window, int key, int scancode, int action, int mods) {
	static_cast<EventManager*>(glfwGetWindowUserPointer(l_window))->KeyCallback(key, scancode, action, mods);
}
void EventManager::KeyCallback(int key, int scancode, int action, int mods) {
	auto itr = callbacks.find(EventType::Key);
	if (itr == callbacks.end())
		return;
	CallbackData data;
	data.window = window;
	data.data = Key_Data({ key, scancode, action, mods });
	for (auto& fun : itr->second)
		fun(data);
}

void EventManager::_MouseButtonCallback(GLFWwindow* l_window, int button, int action, int mods) {
	static_cast<EventManager*>(glfwGetWindowUserPointer(l_window))->MouseButtonCallback(button, action, mods);
}
void EventManager::MouseButtonCallback(int button, int action, int mods) {
	auto itr = callbacks.find(EventType::MouseButton);
	if (itr == callbacks.end())
		return;
	CallbackData data;
	data.window = window;
	data.data = MouseButton_Data({ button, action, mods });
	for (auto& fun : itr->second)
		fun(data);
}

void EventManager::_MouseMoveCallback(GLFWwindow* l_window, double xpos, double ypos){
	static_cast<EventManager*>(glfwGetWindowUserPointer(l_window))->MouseMoveCallback(xpos, ypos);
}
void EventManager::MouseMoveCallback(double xpos, double ypos){
	auto itr = callbacks.find(EventType::MouseMove);
	if (itr == callbacks.end())
		return;
	CallbackData data;
	data.window = window;
	data.data = MouseMove_Data({ xpos, ypos });
	for (auto& fun : itr->second) {
		fun(data);
	}
}

void EventManager::_ScrollCallback(GLFWwindow* l_window, double xoffset, double yoffset) {
	static_cast<EventManager*>(glfwGetWindowUserPointer(l_window))->ScrollCallback(xoffset, yoffset);
}
void EventManager::ScrollCallback(double xoffset, double yoffset) {
	auto itr = callbacks.find(EventType::Scroll);
	if (itr == callbacks.end())
		return;
	CallbackData data;
	data.window = window;
	data.data = Scroll_Data({ xoffset, yoffset });
	for (auto& fun : itr->second) {
		fun(data);
	}
}
