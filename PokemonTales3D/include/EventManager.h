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

enum AZERTY
{
    // -------------------------
    // Alphabet keys
    // -------------------------
    A = GLFW_KEY_Q,
    Z = GLFW_KEY_W,
    E = GLFW_KEY_E,
    R = GLFW_KEY_R,
    T = GLFW_KEY_T,
    Y = GLFW_KEY_Y,
    U = GLFW_KEY_U,
    I = GLFW_KEY_I,
    O = GLFW_KEY_O,
    P = GLFW_KEY_P,

    Q = GLFW_KEY_A,
    S = GLFW_KEY_S,
    D = GLFW_KEY_D,
    F = GLFW_KEY_F,
    G = GLFW_KEY_G,
    H = GLFW_KEY_H,
    J = GLFW_KEY_J,
    K = GLFW_KEY_K,
    L = GLFW_KEY_L,
    M = GLFW_KEY_SEMICOLON,  // 'M' in AZERTY where ';' is in QWERTY

    W = GLFW_KEY_Z,
    X = GLFW_KEY_X,
    C = GLFW_KEY_C,
    V = GLFW_KEY_V,
    B = GLFW_KEY_B,
    N = GLFW_KEY_N,

    // -------------------------
    // Number row (top)
    // -------------------------
    N1 = GLFW_KEY_1,
    N2 = GLFW_KEY_2,
    N3 = GLFW_KEY_3,
    N4 = GLFW_KEY_4,
    N5 = GLFW_KEY_5,
    N6 = GLFW_KEY_6,
    N7 = GLFW_KEY_7,
    N8 = GLFW_KEY_8,
    N9 = GLFW_KEY_9,
    N0 = GLFW_KEY_0,

    // -------------------------
    // Punctuation (approx positions)
    // -------------------------
    MINUS = GLFW_KEY_RIGHT_BRACKET,   // )
    EQUAL = GLFW_KEY_EQUAL,           // =
    SLASH = GLFW_KEY_MINUS,           // -
    BACKSLASH = GLFW_KEY_BACKSLASH,       // remains same
    APOSTROPHE = GLFW_KEY_COMMA,           // ,
    COMMA = GLFW_KEY_M,               // ,
    PERIOD = GLFW_KEY_PERIOD,          // .
    SEMICOLON = GLFW_KEY_SEMICOLON,
    GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,

    // -------------------------
    // Modifier keys
    // -------------------------
    LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
    RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,
    LEFT_CTRL = GLFW_KEY_LEFT_CONTROL,
    RIGHT_CTRL = GLFW_KEY_RIGHT_CONTROL,
    LEFT_ALT = GLFW_KEY_LEFT_ALT,
    RIGHT_ALT = GLFW_KEY_RIGHT_ALT, // AltGr
    LEFT_SUPER = GLFW_KEY_LEFT_SUPER,
    RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER,

    // -------------------------
    // System keys
    // -------------------------
    ESCAPE = GLFW_KEY_ESCAPE,
    TAB = GLFW_KEY_TAB,
    CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
    ENTER = GLFW_KEY_ENTER,
    SPACE = GLFW_KEY_SPACE,
    BACKSPACE = GLFW_KEY_BACKSPACE,

    // -------------------------
    // Function keys
    // -------------------------
    F1 = GLFW_KEY_F1,
    F2 = GLFW_KEY_F2,
    F3 = GLFW_KEY_F3,
    F4 = GLFW_KEY_F4,
    F5 = GLFW_KEY_F5,
    F6 = GLFW_KEY_F6,
    F7 = GLFW_KEY_F7,
    F8 = GLFW_KEY_F8,
    F9 = GLFW_KEY_F9,
    F10 = GLFW_KEY_F10,
    F11 = GLFW_KEY_F11,
    F12 = GLFW_KEY_F12,

    // -------------------------
    // Navigation + Editing
    // -------------------------
    PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,
    SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
    PAUSE = GLFW_KEY_PAUSE,

    INSERT = GLFW_KEY_INSERT,
    DELETE = GLFW_KEY_DELETE,
    HOME = GLFW_KEY_HOME,
    END = GLFW_KEY_END,
    PAGE_UP = GLFW_KEY_PAGE_UP,
    PAGE_DOWN = GLFW_KEY_PAGE_DOWN,

    // -------------------------
    // Arrow keys
    // -------------------------
    ARROW_UP = GLFW_KEY_UP,
    ARROW_DOWN = GLFW_KEY_DOWN,
    ARROW_LEFT = GLFW_KEY_LEFT,
    ARROW_RIGHT = GLFW_KEY_RIGHT,

    // -------------------------
    // Numpad
    // -------------------------
    KP_0 = GLFW_KEY_KP_0,
    KP_1 = GLFW_KEY_KP_1,
    KP_2 = GLFW_KEY_KP_2,
    KP_3 = GLFW_KEY_KP_3,
    KP_4 = GLFW_KEY_KP_4,
    KP_5 = GLFW_KEY_KP_5,
    KP_6 = GLFW_KEY_KP_6,
    KP_7 = GLFW_KEY_KP_7,
    KP_8 = GLFW_KEY_KP_8,
    KP_9 = GLFW_KEY_KP_9,
    KP_DECIMAL = GLFW_KEY_KP_DECIMAL,
    KP_DIVIDE = GLFW_KEY_KP_DIVIDE,
    KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
    KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
    KP_ADD = GLFW_KEY_KP_ADD,
    KP_ENTER = GLFW_KEY_KP_ENTER,
    KP_EQUAL = GLFW_KEY_KP_EQUAL,
};

//Enumeration listing all the supported event type
enum class EventType {
	Key,
	MouseButton,
	MouseMove,
	Scroll
};

//Structure storing Data relative to a Key event
struct Key_Data {
	int key;
	int scancode;
	int action;
	int mods;
};

//Structure storing Data relative to a MouseButton event
struct MouseButton_Data {
	int button;
	int action;
	int	mods;
};

//Structure storing Data relative to a MouseMove event
struct MouseMove_Data {
	double xpos;
	double ypos;
};

//Structure storing Data relative to a Scroll event
struct Scroll_Data {
	double xoffset;
	double yoffset;
};

//Structure storing Data relative to any kind of callback, associated to an event
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

/* Class which handle the event occuring during the game, and call callbacks when it's needed*/
class EventManager {
public:
	EventManager(SharedContext* l_context);
	~EventManager();

	/*Add a callback bind to a event type, the callback is a method from an instance of T
	Params:
	- name : name of the callback
	- etype : type of event bind
	- l_fun : pointer to a method from an instance of T, which have a CallbackData argument
	- l_instance : the instance of T on which the method will be call from
	- stype : the state where the callback is activated, NoneState is for all state
	*/
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

	/*Remove all callbacks named 'name'*/
	void RemoveCallbacks(std::string name);

	/*Set the state of the event manager*/
	void SetState(StateType stype);

	/*These methods are internal to the event manager process, and shall not be used outside*/

	static void _MouseMoveCallback(GLFWwindow* l_window, double xpos, double ypos);
	void MouseMoveCallback(double xpos, double ypos);

	static void _MouseButtonCallback(GLFWwindow* l_window, int button, int action, int mods);
	void MouseButtonCallback(int button, int action, int mods);

	static void _ScrollCallback(GLFWwindow* l_window, double xoffset, double yoffset);
	void ScrollCallback(double xoffset, double yoffset);

	static void _KeyCallback(GLFWwindow* l_window, int key, int scancode, int action, int mods);
	void KeyCallback(int key, int scancode, int action, int mods);

private:
	GLFWwindow* window;
	SharedContext* context;

	CallbackContainer callbacks;
	StateCallbackContainer* currCallbacks;

	StateType state;
};