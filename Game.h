#pragma once

class MainMenu;

class Game {
private:
    //Variables
    sf::RenderWindow* window;
    sf::Event ev;
    sf::VideoMode videoMode;
    sf::Clock dtClock;
    float dt;

    //Loading Animation
    sf::Font font;
    sf::Text loadingTextName;
    sf::Text loadingTextPresents;
    sf::Clock fadeClock;
    float opacity;
    int fadeDirection;
    bool loadFinished;

    //Main Menu
    MainMenu& mainMenu;
    InitGameStart& initGameStart;
    InGame* inGame;

    //Background music
    sf::Music backgroundMusic;
    bool playOnce;

    //Private functions
    void initVariables();
    void initLoadingText();
    void initWindow();
    void pollEvents();

    void playMusic();

    void updateLoadingText();
    void updateMainMenu() const;
    void updateInitGameStart();
    void update();

    void renderLoadingText(sf::RenderTarget& target) const;
    void renderMainMenu(sf::RenderTarget& target) const;
    void render() const;

public:
    explicit Game(MainMenu& mainMenu, InitGameStart& initGameStart);

    ~Game();

    void initMusic();

    void resumeMusic();


    void stopMusic();

    //Accessors
    sf::Event& getEv();

    //Functions
    void run();
};
