#include "EventManager.h"

KeyboardEvent::KeyboardEvent(EType l_type, sf::Keyboard::Key l_key) : type(l_type), key(l_key){}
bool KeyboardEvent::match(const sf::Event& event) {
	if (type == EType::Pressed)
		return event.is<sf::Event::KeyPressed>() && event.getIf<sf::Event::KeyPressed>()->code == key;
	return event.is<sf::Event::KeyReleased>() && event.getIf<sf::Event::KeyReleased>()->code == key;
}
bool KeyboardEvent::match() {
	return type == EType::IsPressed && sf::Keyboard::isKeyPressed(key);
}

WindowEvent::WindowEvent(EType l_type) : type(l_type){}
bool WindowEvent::match(const sf::Event& event) {
	if (type == EType::Closed)
		return event.is<sf::Event::Closed>();
	if (type == EType::MouseMove)
		return event.is<sf::Event::MouseMoved>();
	return false;
}
bool WindowEvent::match() {
	return false;
}

EventManager::EventManager() : hasFocus(true){
	
}
EventManager::~EventManager(){
	for (auto itr : bindings) {
		delete itr.second;
		itr.second = nullptr;
	}
}

bool EventManager::AddBinding(Binding* l_bindings){
	if (bindings.find(l_bindings->name) != bindings.end()) {
		return false;
	}
	return bindings.emplace(l_bindings->name, l_bindings).second;
}
bool EventManager::RemoveBinding(std::string l_name){
	auto itr = bindings.find(l_name);
	if (itr == bindings.end()) { return false; }
	delete itr->second;
	bindings.erase(itr);
	return true;
}
void EventManager::SetFocus(bool l_focus) { hasFocus = l_focus; }

void EventManager::HandleEvent(const sf::Event l_event){
	for (auto& b_itr : bindings) {
		Binding* bind = b_itr.second;
		for (ASfmlEvent* event : bind->events) {
			if (event->match(l_event)) {
				++(bind->c);
			}
		}
	}
}

void EventManager::Update() {
	if (!hasFocus) { return; }

	for (auto& b_itr : bindings) {
		Binding* bind = b_itr.second;
		for (ASfmlEvent* event : bind->events) {
			if (event->match()) {
				++(bind->c);
			}
		}

		if (bind->events.size() == bind->c) {
			auto callItr = callbacks.find(bind->name);
			if (callItr != callbacks.end()) {
				callItr->second();
			}
		}
		bind->c = 0;
	}
}