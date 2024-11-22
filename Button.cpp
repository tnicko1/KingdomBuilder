#include "pch.h"
#include "Button.h"

Button::Button(const sf::Vector2f& position,
               const sf::Vector2f& size,
               const std::string& buttonText,
               const sf::Font& buttonFont,
               const sf::Color& idleCol,
               const sf::Color& hoverCol,
               const sf::Color& activeCol)
    : font(buttonFont)
{
    shape.setPosition(position);
    shape.setSize(size);

    idleColor = idleCol;
    hoverColor = hoverCol;
    activeColor = activeCol;
    shape.setFillColor(idleColor);

    text.setFont(font);
    text.setString(buttonText);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    // Center text
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
    text.setPosition(
        position.x + size.x / 2.0f,
        position.y + size.y / 2.0f - textBounds.height / 2.0f
    );

    // Load hover sound
    if (!hoverBuffer.loadFromFile("Effects/button-hover.wav")) {
        std::cout << "Error loading hover sound!" << std::endl;
    }
    hoverSound.setBuffer(hoverBuffer);
    hoverSound.setVolume(100);  // Default volume

    isHovered = false;
    wasHovered = false;
    isPressed = false;
}

bool Button::isClicked(const sf::Vector2i& mousePos) {
    sf::FloatRect bounds = shape.getGlobalBounds();
    bool mouseInButton = bounds.contains(static_cast<float>(mousePos.x),
                                       static_cast<float>(mousePos.y));

    // Check if mouse is over button and left mouse button is pressed
    if (mouseInButton && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        return true;
    }
    return false;
}

void Button::setVolume(float volume) {
    hoverSound.setVolume(volume);
}

void Button::update(const sf::Vector2i& mousePos) {
    sf::FloatRect bounds = shape.getGlobalBounds();
    isHovered = bounds.contains(static_cast<float>(mousePos.x),
                              static_cast<float>(mousePos.y));

    // Play sound only when we first hover over the button
    if (isHovered && !wasHovered) {
        hoverSound.play();
    }

    wasHovered = isHovered;

    if (isHovered) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            shape.setFillColor(activeColor);
        } else {
            shape.setFillColor(hoverColor);
        }
    } else {
        shape.setFillColor(idleColor);
    }
}

void Button::draw(sf::RenderTarget& target) {
    target.draw(shape);
    target.draw(text);
}