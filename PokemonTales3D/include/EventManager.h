#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <variant>
#include <map>
#include <functional>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "StateManager.h"

struct EnumHash {
	template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

enum class EventType {
	Key,
	MouseButton,
	MouseMove,
	Scroll
};

struct Key_Data {
	int key;
	int scancode;
	int action;
	int mods;
};

struct MouseButton_Data {
	int button;
	int action;
	int	mods;
};

struct MouseMove_Data {
	double xpos;
	double ypos;
};

struct Scroll_Data {
	double xoffset;
	double yoffset;
};

struct CallbackData {
	GLFWwindow* window;
	std::variant<
		Key_Data,
		MouseButton_Data,
		MouseMove_Data,
		Scroll_Data
	> data;
};

using Callback = std::pair<std::string, std::function<void(CallbackData)>>;
using CallbackVector = std::vector<Callback>;
using StateCallbackContainer = std::unordered_map<EventType, CallbackVector, EnumHash>;
using CallbackContainer = std::unordered_map<StateType, StateCallbackContainer, EnumHash>;

class EventManager {
public:
	EventManager(SharedContext* l_context);
	~EventManager();

	template <class T>
	void AddCallback(std::string name, EventType etype, void(T::* l_fun)(CallbackData)
		, T* l_instance, StateType stype = StateType::NoneState) {
		auto temp = std::bind(l_fun, l_instance, std::placeholders::_1);
		auto callback = std::make_pair(name, temp);
		auto stateItr = callbacks.find(stype);
		if (stateItr == callbacks.end())
			stateItr = callbacks.insert(std::make_pair(stype, StateCallbackContainer())).first;
		auto& stateCallback = stateItr->second;
		auto itr = stateCallback.find(etype);
		if (itr == stateCallback.end())
			itr = stateCallback.insert(std::make_pair(etype, CallbackVector())).first;
		itr->second.push_back(callback);
	}

	/*Give a instance and delete all callbacks bound to this instance*/
	void RemoveCallbacks(std::string name);

	static void _MouseMoveCallback(GLFWwindow* l_window, double xpos, double ypos);
	void MouseMoveCallback(double xpos, double ypos);

	static void _MouseButtonCallback(GLFWwindow* l_window, int button, int action, int mods);
	void MouseButtonCallback(int button, int action, int mods);

	static void _ScrollCallback(GLFWwindow* l_window, double xoffset, double yoffset);
	void ScrollCallback(double xoffset, double yoffset);

	static void _KeyCallback(GLFWwindow* l_window, int key, int scancode, int action, int mods);
	void KeyCallback(int key, int scancode, int action, int mods);

	void SetState(StateType stype);

private:
	GLFWwindow* window;
	SharedContext* context;

	CallbackContainer callbacks;
	StateCallbackContainer* currCallbacks;

	StateType state;
};