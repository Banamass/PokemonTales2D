#pragma once

#include "SharedTypes.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <sstream>

class ASfmlEvent {
public:
    virtual bool match() = 0;
    virtual bool match(const sf::Event& event) = 0;
};

class KeyboardEvent : public ASfmlEvent{
public:
    enum EType { Pressed, Released, IsPressed };

    KeyboardEvent(EType l_type, sf::Keyboard::Key l_key);
    bool match();
    bool match(const sf::Event& event);

private:
    EType type;
    sf::Keyboard::Key key;
};

class WindowEvent : public ASfmlEvent {
public:
    enum EType { Closed, MouseMove };

    WindowEvent(EType l_type);
    bool match();
    bool match(const sf::Event& event);

private:
    EType type;
};

using Events = std::vector<ASfmlEvent*>;

struct Binding {
    Binding(const std::string& l_name)
        : name(l_name), c(0) {}

    ~Binding() {
        for (ASfmlEvent* event : events) {
            delete event;
        }
    }

    void BindEvent(ASfmlEvent* event) {
        events.push_back(event);
    }

    Events events;
    std::string name;
    int c; // count of events that have happened
};

using Bindings = std::unordered_map<std::string, Binding*>;

using CallbackContainer = std::unordered_map<std::string, std::function<void()> >;

class EventManager
{
public:
    EventManager();
    virtual ~EventManager();

    void SetFocus(bool l_focus);

    template<class T>
    bool AddCallback(const std::string& l_name, void(T::*l_func)(void), T* l_instance) {
        auto temp = std::bind(l_func, l_instance);
        return callbacks.emplace(std::make_pair(l_name, temp)).second;
    }

    bool RemoveCallback(const std::string& l_name) {
        auto itr = callbacks.find(l_name);
        if (itr == callbacks.end()) {
            return false;
        }
        callbacks.erase(l_name);
        return true;
    }

    bool AddBinding(Binding* l_bindings);
    bool RemoveBinding(std::string l_name);

    void HandleEvent(const sf::Event l_event);

    void Update();

protected:
    Bindings bindings;
    CallbackContainer callbacks;
    bool hasFocus;
};

