#pragma once

#include "pch.h"

class InteractiveButton {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Font font;
    sf::Color idleColor;
    sf::Color hoverColor;
    bool keyHeld;
    bool isHeld; // variable to track mouse click state

    const sf::Vector2f& position;
    const sf::Vector2f& size;

    std::string userInput;

    bool isHovered;
    bool wasHovered;

    bool isEditing;
    std::string buttonText;
    sf::SoundBuffer hoverBuffer;
    sf::Sound hoverSound;

    const size_t maxCharacterLimit = 20; // Maximum character limit

public:
    InteractiveButton(const sf::Vector2f& position, const sf::Vector2f& size, std::string initialText, const std::string& fontPath, const sf::Color idleColor, const sf::Color hoverColor)
        : idleColor(idleColor), hoverColor(hoverColor), keyHeld(false), isHeld(false), position(position), size(size), isEditing(false), buttonText(std::move(initialText)) {
        // Load font
        if (!font.loadFromFile(fontPath)) {
            std::cerr << "Failed to load font: " << fontPath << std::endl;
        }

        isHovered = false;
        wasHovered = false;

        // Set up button shape
        shape.setPosition(position);
        shape.setSize(size);
        shape.setFillColor(idleColor);

        // Set up button text
        text.setFont(font);
        text.setString(buttonText);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);

        // Center text
        sf::FloatRect textBounds = text.getGlobalBounds();
        text.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
        text.setPosition(
            position.x + size.x / 2.0f,
            position.y + size.y / 2.0f - textBounds.height / 2.0f
        );

        // Load hover sound
        if (!hoverBuffer.loadFromFile("Effects/button-hover.wav")) {
            std::cerr << "Error loading hover sound!" << std::endl;
        }
        hoverSound.setBuffer(hoverBuffer);
        hoverSound.setVolume(50);
    }

    bool isClicked(const sf::Vector2i& mousePos) const {
        const sf::FloatRect bounds = shape.getGlobalBounds();

        // Check if mouse is over button and left mouse button is pressed
        if (const bool mouseInButton = bounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)); mouseInButton && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            return true;
        }
        return false;
    }

    void setVolume(const float volume) {
        hoverSound.setVolume(volume);
    }

    std::string getUserInput() {
        return this->userInput;
    }

    void update(const sf::Vector2i& mousePos, const sf::Event& event) {
        const sf::FloatRect bounds = shape.getGlobalBounds();
        const bool isHovered = bounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        // Play sound only when we first hover over the button
        if (isHovered && !wasHovered) {
            hoverSound.play();
        }
        wasHovered = isHovered;

        if (isHovered) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                shape.setFillColor(sf::Color(70, 70, 70));
            } else {
                shape.setFillColor(hoverColor);
            }
        } else {
            shape.setFillColor(idleColor);
        }

        if (isEditing) {
            shape.setFillColor(sf::Color(30, 30, 30, 200));
        }

        // Handle button click only if mouse is not held, and we are not already editing
        if (isClicked(mousePos)) {
            if (!isHeld && !isEditing) {
                isEditing = true;
                text.setString(""); // Clear the text
            }
            isHeld = true; // Set isHeld so the click doesn't register multiple times
        } else {
            isHeld = false; // Reset isHeld when mouse is not clicked
        }

        // Update the input field with key press/release handling
        if (isEditing) {
            // Handle key input only if no key is currently held down
            if (event.type == sf::Event::TextEntered && !keyHeld) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == 8) { // Backspace
                        if (!text.getString().isEmpty()) {
                            text.setString(text.getString().substring(0, text.getString().getSize() - 1));
                        }
                    } else if (text.getString().getSize() < maxCharacterLimit) { // Add character only if under limit
                        text.setString(text.getString() + static_cast<char>(event.text.unicode));
                    }
                }
                keyHeld = true;  // Set keyHeld to true after processing input
            }

            // Reset keyHeld when key is released
            if (event.type == sf::Event::KeyReleased) {
                keyHeld = false;
                if (event.key.code == sf::Keyboard::Enter) {
                    isEditing = false;
                    buttonText = text.getString(); // Update the button text
                    this->userInput = buttonText;
                    text.setString(buttonText);
                    text.setOrigin(  // Center text
                        text.getGlobalBounds().width / 2.0f,
                        text.getGlobalBounds().height / 2.0f
                    );
                    text.setPosition(   // Center text
                        shape.getGlobalBounds().width / 2.0f + shape.getGlobalBounds().left,
                        shape.getGlobalBounds().height / 2.0f + shape.getGlobalBounds().top - text.getGlobalBounds().height / 2.0f
                    );
                }
            }
        }
    }

    void draw(sf::RenderTarget& target) const {
        target.draw(shape);
        target.draw(text);
    }
};
