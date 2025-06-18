#include "Window.h"

void Window::Setup(const std::string& l_title, const sf::Vector2u l_size) {
	windowTitle = l_title;
	windowSize = l_size;
	isFullscreen = false;
	isDone = false;
	isFocused = true;

	SetCallbacks();

	Create();
}

void Window::SetCallbacks() {
	Binding* b_close = new Binding("CloseButton");
	b_close->BindEvent(new WindowEvent(WindowEvent::EType::Closed));
	eventManager.AddBinding(b_close);

	eventManager.AddCallback("CloseButton", &Window::Close, this);

	Binding* b_escape = new Binding("EscapeKey");
	b_escape->BindEvent(new KeyboardEvent(KeyboardEvent::EType::Pressed, sf::Keyboard::Key::Escape));
	eventManager.AddBinding(b_escape);

	eventManager.AddCallback("EscapeKey", &Window::Close, this);

	Binding* b_ctrl = new Binding("LCtrlFKey");
	b_ctrl->BindEvent(new KeyboardEvent(KeyboardEvent::EType::IsPressed, sf::Keyboard::Key::LControl));
	b_ctrl->BindEvent(new KeyboardEvent(KeyboardEvent::EType::Pressed, sf::Keyboard::Key::F));
	eventManager.AddBinding(b_ctrl);

	eventManager.AddCallback("LCtrlFKey", &Window::ToggleFullscreen, this);
}

Window::Window(const std::string& l_title, const sf::Vector2u l_size, SharedContext* context) {
	Setup(l_title, l_size);
	context->eventManager = &eventManager;
}
Window::~Window() {
	Destroy();
}

void Window::BeginDraw(){
	window.clear(sf::Color::Black);
}
void Window::Draw(sf::Drawable& l_drawable) {
	window.draw(l_drawable);
}
void Window::EndDraw(){
	window.display();
}
void Window::Update(){
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::FocusLost>()) {
			isFocused = false;
			eventManager.SetFocus(false);
		}
		else if (event->is<sf::Event::FocusGained>()) {
			isFocused = true;
			eventManager.SetFocus(true);
		}
		eventManager.HandleEvent(event.value());
	}
	eventManager.Update();
}

bool Window::IsDone() { return isDone; }
bool Window::IsFullscreen() { return isFullscreen; }
bool Window::IsFocused() { return isFocused; }
sf::Vector2u Window::GetWindowSize() { return windowSize; }
sf::RenderWindow* Window::GetRenderWindow() { return &window; }
sf::Vector2i Window::GetMousePos() {
	return sf::Mouse::getPosition(window);
}
sf::FloatRect Window::GetViewSpace() {
	sf::Vector2f viewCenter = window.getView().getCenter();
	sf::Vector2f viewSize = window.getView().getSize();
	sf::Vector2f viewSizeHalf(viewSize.x / 2, viewSize.y / 2);
	sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);
	return viewSpace;
}

void Window::ToggleFullscreen() {
	isFullscreen = !isFullscreen;
	Destroy();
	Create();
}
void Window::Close(){
	isDone = true;
}

void Window::Destroy() {
	window.close();
}
void Window::Create() {
	auto state = (isFullscreen ? sf::State::Fullscreen : sf::State::Windowed);
	window.create(sf::VideoMode({ windowSize.x, windowSize.y }), windowTitle, sf::Style::Default, state);
	window.setKeyRepeatEnabled(false); //Press a key will produce only one sf::Event::KeyPressed
}