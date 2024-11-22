#pragma once
class InitGameStart {
    sf::VideoMode videoMode;
    sf::Font font;

    sf::Event ev;

    sf::RectangleShape buttonBackground;

    InteractiveButton* chooseKingdomButton;
    Button* playButton;
    Button* returnButton;


    bool endGame;
    bool playGame;
    bool returnButtonClicked;

    bool isUpdating;
    bool isRendering;

    void initVariables();
    void initBackground();
    void initButtons();

    void deleteButtons() const;
public:
    InitGameStart();
    ~InitGameStart();

    std::string userKingdomName;

    bool getEndGame() const;
    bool getPlayGame() const;
    bool getReturnButtonClicked() const;

    void setReturnButtonClicked(bool clicked);

    void setPlayGame(bool play);

    void setEvt(const sf::Event& evt);

    void stopRender();
    void stopUpdate();

    void update();
    void render(sf::RenderTarget& target) const;

    void startRender();

    void startUpdate();
};
