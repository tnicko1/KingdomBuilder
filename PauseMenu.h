#pragma once
#include "pch.h"

class PauseMenu {
private:
    sf::RenderWindow* window;
    sf::Font font;
    sf::RectangleShape background;
    sf::Text resumeText;
    sf::Text exitText;
    sf::Text* selectedText;

    bool* isPausedPtr;

    void initVariables();
    void initText();

public:
    explicit PauseMenu(sf::RenderWindow* window, bool* isPaused);
    ~PauseMenu() = default;


    void update(const sf::Event& event);
    void render() const;
    void handleMouseMove(sf::Vector2i mousePos);
    void handleClick(sf::Vector2i mousePos);
    bool isTextSelected(const sf::Text& text, sf::Vector2i mousePos) const;
};