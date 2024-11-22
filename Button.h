#pragma once

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    const sf::Font& font;

    // Sound
    sf::SoundBuffer hoverBuffer;
    sf::Sound hoverSound;
    bool wasHovered;  // Track previous hover state

    // Button states
    bool isHovered;
    bool isPressed;
    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color activeColor;

public:
    Button(const sf::Vector2f& position,
           const sf::Vector2f& size,
           const std::string& buttonText,
           const sf::Font& buttonFont,
           const sf::Color& idleCol = sf::Color(100, 100, 100),
           const sf::Color& hoverCol = sf::Color(150, 150, 150),
           const sf::Color& activeCol = sf::Color(70, 70, 70));

    // Returns true if button was clicked
    bool isClicked(const sf::Vector2i& mousePos);
    void setVolume(float volume); // 0 to 100
    void update(const sf::Vector2i& mousePos);
    void draw(sf::RenderTarget& target);
};