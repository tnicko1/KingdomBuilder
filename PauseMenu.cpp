#include "PauseMenu.h"
#include "pch.h"

PauseMenu::PauseMenu(sf::RenderWindow* window, bool* isPaused)
    : window(window), selectedText(nullptr), isPausedPtr(isPaused) {  // Update constructor
    initVariables();
    initText();
}

void PauseMenu::initVariables() {
    // Create semi-transparent dark background
    background.setSize(sf::Vector2f(window->getSize()));
    background.setFillColor(sf::Color(0, 0, 0, 128));

    if (!font.loadFromFile("Fonts/metropolis/Metropolis-SemiBold.otf")) {
        // Handle font loading error
        throw std::runtime_error("Failed to load font");
    }
}

void PauseMenu::initText() {
    // Initialize resume text
    resumeText.setFont(font);
    resumeText.setString("Resume");
    resumeText.setCharacterSize(40);
    resumeText.setFillColor(sf::Color::White);
    resumeText.setPosition(
        window->getSize().x / 2.0f - resumeText.getGlobalBounds().width / 2.0f,
        window->getSize().y / 2.0f - 50.0f
    );

    // Initialize exit text
    exitText.setFont(font);
    exitText.setString("Exit");
    exitText.setCharacterSize(40);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(
        window->getSize().x / 2.0f - exitText.getGlobalBounds().width / 2.0f,
        window->getSize().y / 2.0f + 50.0f
    );
}

void PauseMenu::handleMouseMove(sf::Vector2i mousePos) {
    // Reset all text colors
    resumeText.setFillColor(sf::Color::White);
    exitText.setFillColor(sf::Color::White);

    // Check if mouse is over any text
    if (isTextSelected(resumeText, mousePos)) {
        resumeText.setFillColor(sf::Color::Yellow);
        selectedText = &resumeText;
    }
    else if (isTextSelected(exitText, mousePos)) {
        exitText.setFillColor(sf::Color::Yellow);
        selectedText = &exitText;
    }
    else {
        selectedText = nullptr;
    }
}

void PauseMenu::handleClick(sf::Vector2i mousePos) {
    if (isTextSelected(resumeText, mousePos)) {
        // Resume game logic here
        *isPausedPtr = false;
    }
    else if (isTextSelected(exitText, mousePos)) {
        window->close();
    }
}

bool PauseMenu::isTextSelected(const sf::Text& text, sf::Vector2i mousePos) const {
    return text.getGlobalBounds().contains(window->mapPixelToCoords(mousePos));
}

void PauseMenu::update(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        handleMouseMove(sf::Mouse::getPosition(*window));
    }
    else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            handleClick(sf::Mouse::getPosition(*window));
        }
    }
}

void PauseMenu::render() const {
    window->draw(background);
    window->draw(resumeText);
    window->draw(exitText);
}