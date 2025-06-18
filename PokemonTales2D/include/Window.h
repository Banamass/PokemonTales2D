#pragma once

#include "EventManager.h"

class Window
{
public:
	Window(const std::string& l_title, const sf::Vector2u l_size, SharedContext* context);
	virtual ~Window();

	void BeginDraw();
	void Draw(sf::Drawable& l_drawable);
	void EndDraw();
	void Update();

	bool IsDone();
	bool IsFullscreen();
	bool IsFocused();
	sf::Vector2u GetWindowSize();
	sf::RenderWindow* GetRenderWindow();
	sf::FloatRect GetViewSpace();
	sf::Vector2i GetMousePos();

	void ToggleFullscreen();
	void Close();

protected:
	void Setup(const std::string& l_title, const sf::Vector2u l_size);
	void SetCallbacks();

	void Destroy();
	void Create();

	sf::RenderWindow window;
	sf::Vector2u windowSize;
	std::string windowTitle;

	bool isDone;
	bool isFullscreen;
	bool isFocused;

	EventManager eventManager;
};

