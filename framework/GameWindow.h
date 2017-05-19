#pragma once
#include "framework/event/EventManager.h"


class GameWindow
{
public:
	GameWindow();
	GameWindow(const std::string& l_title, const sf::Vector2u& l_size);
	virtual ~GameWindow();

public:
	void beginDraw();// clear window
	void endDraw();// display the changes

	void update();

	bool isDone() const { return m_isDone; };
	bool isFullscreen() const { return m_isFullscreen; };
	sf::Vector2u getWindowSize() { return m_windowSize; };
	sf::RenderWindow* getWindow() { return &m_window; }
	

	void draw(sf::Drawable& l_drawable);

	//event manager
	bool isFocused() const { return m_isFocused; };
	EventManager* getEventManager() { return &m_eventManager; };
	void toggleFullscreen(EventInfo* info);
	void changeResolution(EventInfo* info);
	void close(EventInfo* details = nullptr);


private:
	void setup(const std::string& l_title, const sf::Vector2u& l_size);
	void destroy();
	void create();

private:
	sf::RenderWindow m_window;
	sf::Vector2u m_windowSize;
	std::string m_windowTitle;

	EventManager m_eventManager;


	uint32 m_resolutionIndex;
	bool m_isDone;
	bool m_isFullscreen;
	bool m_isFocused;
};