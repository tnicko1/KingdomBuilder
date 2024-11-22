#pragma once
#include "PauseMenu.h"

class PauseMenu;
class Toolbox;

class InGame {
private:
    // Window and view components
    sf::RenderWindow* window;
    mutable sf::View gameView;  // Marked mutable since it needs to change even in const methods

    sf::SoundBuffer hoverBuffer;
    sf::Sound hoverSound;
    bool wasHovered;  // Track previous hover state

    // Button states
    bool isPressed;

    //Textures
    sf::Texture mapTexture;
    sf::Sprite mapSprite;

    sf::Texture elvenIslesTexture;
    sf::Sprite elvenIslesSprite;

    sf::Texture iceReachTexture;
    sf::Sprite iceReachSprite;

    sf::Texture thrymholdTexture;
    sf::Sprite thrymholdSprite;

    sf::Texture solmareTexture;
    sf::Sprite solmareSprite;

    sf::Texture lowenvaleTexture;
    sf::Sprite lowenvaleSprite;

    sf::Texture embercrownTexture;
    sf::Sprite embercrownSprite;

    sf::Texture everdellTexture;
    sf::Sprite everdellSprite;

    sf::Texture concordiaTexture;
    sf::Sprite concordiaSprite;

    //Text
    sf::Font font;
    sf::Font battleFont;
    sf::Text elvenIslesTitle;
    sf::Text iceReachTitle;
    sf::Text thrymholdTitle;
    sf::Text solmareTitle;
    sf::Text lowenvaleTitle;
    sf::Text embercrownTitle;
    sf::Text everdellTitle;
    sf::Text concordiaTitle;

    // Choose Kingdom Text
    sf::RectangleShape chooseKingdomBackground;
    sf::Text chooseKingdomText;
    bool kingdomChosen;
    sf::Text* playerKingdom;
    sf::Color playerKingdomColor;

    bool isPaused;
    PauseMenu* pauseMenu;

    //Sprite Pointers
    sf::Text* hoveredText;
    sf::Text* clickedText;


    sf::Text userKingdomName;
    bool firstClick;

    // Dice animation properties
    sf::Clock diceAnimationClock;
    bool isDiceAnimating;
    int currentDiceFrame;
    std::vector<sf::Texture> diceTextures;
    sf::Sprite diceSprite;
    sf::Vector2f dicePosition;
    const float DICE_ANIMATION_FRAME_DURATION = 0.1f; // seconds per frame
    const int TOTAL_DICE_FRAMES = 6;


    std::map<sf::Text*, sf::Color> currentTerritoryColors;  // Add this to track current colors
    sf::Text* previousClickedText;
    sf::Color previousClickedColor;
    sf::Color currentClickedColor;
    bool battleWon;  // Add this to track battle outcome

    sf::Clock resultDisplayClock;
    bool isDiceResultShowing;
    static constexpr float DICE_RESULT_DISPLAY_DURATION = 1.0f; // Time in seconds to show the final dice result
    int attackerScore;
    int defenderScore;
    static constexpr float BATTLE_RESULT_DISPLAY_DURATION = 2.0f; // Increased to show both rolls
    std::string battleResultText;
    sf::Text battleResult;

    sf::Sprite attackerDiceSprite;
    sf::Sprite defenderDiceSprite;
    sf::Vector2f attackerDicePosition;
    sf::Vector2f defenderDicePosition;

    //Game Over
    bool isGameOver;
    bool checkGameOver();
    void renderGameOver();

    void renderVictory();

    sf::Text gameOverText;
    sf::RectangleShape gameOverBackground;
    sf::RectangleShape exitButton;
    sf::Text exitButtonText;

    bool isVictory;
    sf::Text victoryText;
    sf::RectangleShape victoryBackground;


    void loadDiceTextures();
    void updateDiceAnimation();
    void startDiceAnimation(const sf::Vector2f& position);
    void renderDiceAnimation() const;

    // Private member functions
    void initVariables();
    void initText();
    void initChooseKingdom();
    void centerView();
    void handleInput(const sf::Event& event);

    sf::Color getCurrentColor(sf::Text *text);

    sf::Sprite &getAssociatedSprite(sf::Text *text);

    void handleTextHover(sf::Text &text, sf::Sprite &associatedSprite, const sf::Event &event);

    void handleTextClick(sf::Text &text, sf::Sprite &associatedSprite, const sf::Event &event);

    float scaleFactor;
    sf::Vector2f baseResolution;
    void calculateScaleFactor();
    void updatePositions();

    static constexpr float BASE_SCALE = 1.35f;
    static constexpr float HOVER_SCALE_MULTIPLIER = 1.01f;
public:
    // Constructor/Destructor
    explicit InGame(sf::RenderWindow* window);
    ~InGame();

    // Public member functions
    void loadTexture(const std::string& texturePath);
    void initUserKingdomName(const std::string &);

    void update(const sf::Event& event);

    void renderSprites();
    void renderText() const;

    void render();  // Changed to not take a target parameter
};