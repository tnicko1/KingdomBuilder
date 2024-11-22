#pragma once

class MainMenu {
private:
    //Variables
    sf::VideoMode videoMode;
    sf::Font font;
    bool endGame;
    bool startGameBool;
    bool returnButtonClicked;

    bool isUpdating;
    bool isRendering;

    //Main menu
    sf::Texture mainMenuTexture;
    sf::Sprite mainMenuSprite;
    sf::RectangleShape buttonBackground;
    sf::Texture titleTexture;
    sf::Sprite titleSprite;

    //Buttons
    Button* startButton;
    Button* howToPlayButton;
    Button* creditsButton;
    Button* exitButton;

    //How to Play
    bool showHowToPlay;
    std::vector<sf::Texture> tutorialTextures;
    sf::Sprite tutorialSprite;
    size_t currentTutorialPage;
    Button* nextButton;
    Button* prevButton;
    Button* backButton;

    // Credits
    bool showCredits;
    sf::Text creditsText;
    Button* creditsReturnButton;

    // Click cooldown
    sf::Clock buttonCooldown;
    float cooldownTime;
    bool canClick;

    void initVariables();

    void initHowToPlayButtons();

    void initCreditsButton();

    void setCreditsText(const std::string &credits);

    void initBackground();
    void initButtons();

    void startGame();

    void deleteButtons() const;
public:
    MainMenu();
    ~MainMenu();

    //Accessors
    bool getEndGame() const;
    bool getGameStart() const;

    void setGameStart(bool start);
    void setReturnButtonClicked(bool clicked);

    void loadTutorialImages(const std::vector<std::string> &imagePaths);

    void updateTutorialPage();

    void stopUpdate();
    void stopRender();

    //Functions
    void update();

    void setCooldownTime(float seconds);

    void render(sf::RenderTarget& target) const;

    void startRender();

    void startUpdate();
};
