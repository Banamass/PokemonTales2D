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

struct EnumHash {
	template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

enum class EventType {
	MouseMove,
	Scroll
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
		MouseMove_Data,
		Scroll_Data
	> data;
};

using CallbackVector = std::vector<std::function<void(CallbackData)>>;
using CallbackContainer = std::unordered_map<EventType, CallbackVector, EnumHash>;

class EventManager {
public:
	EventManager(Window* l_window);
	~EventManager();

	template <class T>
	void AddCallback(EventType type, void(T::* l_fun)(CallbackData), T* l_instance) {
		auto temp = std::bind(l_fun, l_instance, std::placeholders::_1);
		auto itr = callbacks.find(type);
		if (itr == callbacks.end())
			itr = callbacks.insert(std::make_pair(type, CallbackVector())).first;
		itr->second.push_back(temp);
	}

	static void _MouseMoveCallback(GLFWwindow* l_window, double xpos, double ypos);
	void MouseMoveCallback(double xpos, double ypos);

	static void _ScrollCallback(GLFWwindow* l_window, double xoffset, double yoffset);
	void ScrollCallback(double xoffset, double yoffset);

private:
	GLFWwindow* window;

	CallbackContainer callbacks;
};