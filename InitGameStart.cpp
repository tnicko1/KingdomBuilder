#include "pch.h"
#include "InitGameStart.h"

void InitGameStart::initVariables() {
    this->videoMode = sf::VideoMode::getDesktopMode();
    this->font.loadFromFile("Fonts/metropolis/Metropolis-SemiBold.otf");
    this->endGame = false;
    this->playGame = false;
    this->returnButtonClicked = false;
    this->isUpdating = true;
    this->isRendering = true;
    this->userKingdomName = "";
}

void InitGameStart::initBackground() {
    this->buttonBackground.setSize(sf::Vector2f(static_cast<float>(this->videoMode.width) / 4.f, static_cast<float>(this->videoMode.height)));
    this->buttonBackground.setFillColor(sf::Color(0, 0, 0, 150));
    this->buttonBackground.setPosition(static_cast<float>(this->videoMode.width) / 4.f + this->buttonBackground.getGlobalBounds().width / 2.f, 0);
}

void InitGameStart::initButtons() {
    this->chooseKingdomButton = new InteractiveButton(
    sf::Vector2f(this->buttonBackground.getGlobalBounds().width + this->buttonBackground.getGlobalBounds().width / 2.f, this->buttonBackground.getLocalBounds().height / 2.f - 100.f),
    sf::Vector2f(this->buttonBackground.getGlobalBounds().width, 50.0f),
    "Enter Kingdom Name",
    "Fonts/metropolis/Metropolis-SemiBold.otf", sf::Color(0, 0, 0, 0),
    sf::Color(0, 0, 0, 150)
    );

    this->playButton = new Button(
        sf::Vector2f(this->buttonBackground.getGlobalBounds().width + this->buttonBackground.getGlobalBounds().width / 2.f, this->buttonBackground.getLocalBounds().height / 2.f - 50.f),
        sf::Vector2f(this->buttonBackground.getGlobalBounds().width, 50.f),
        "Play Game",
        this->font,
        sf::Color(0, 0, 0, 0),
        sf::Color(0, 0, 0, 150)
    );

    this->returnButton = new Button(
        sf::Vector2f(this->buttonBackground.getGlobalBounds().width + this->buttonBackground.getGlobalBounds().width / 2.f, this->buttonBackground.getLocalBounds().height / 2.f + 50.f),
        sf::Vector2f(this->buttonBackground.getGlobalBounds().width, 50.f),
        "Return to Main Menu",
        this->font,
        sf::Color(0, 0, 0, 0),
        sf::Color(0, 0, 0, 150)
    );
}

void InitGameStart::deleteButtons() const {
    delete this->chooseKingdomButton;
    delete this->playButton;
    delete this->returnButton;
}


InitGameStart::InitGameStart() {
    this->initVariables();
    this->initBackground();
    this->initButtons();
}

InitGameStart::~InitGameStart() {
    this->deleteButtons();
}

bool InitGameStart::getEndGame() const {
    return this->endGame;
}

bool InitGameStart::getPlayGame() const {
    return this->playGame;
}

bool InitGameStart::getReturnButtonClicked() const {
    return this->returnButtonClicked;
}

void InitGameStart::setReturnButtonClicked(const bool clicked) {
    this->returnButtonClicked = clicked;
}

void InitGameStart::setPlayGame(bool play) {
    this->playGame = play;
}

void InitGameStart::setEvt(const sf::Event &evt) {
    this->ev = evt;
}

void InitGameStart::stopRender() {
    this->isRendering = false;
}

void InitGameStart::stopUpdate() {
    this->isUpdating = false;
}


void InitGameStart::update() {
    if (isUpdating) {
        const sf::Vector2i mousePos = sf::Mouse::getPosition();
        this->chooseKingdomButton->update(mousePos, ev);

        this->playButton->update(mousePos);
        if (this->playButton->isClicked(mousePos)) {
            this->userKingdomName = chooseKingdomButton->getUserInput(); // Get user input for kingdom name
            this->playGame = true;
        }

        this->returnButton->update(mousePos);
        if (this->returnButton->isClicked(mousePos)) {
            this->returnButtonClicked = true;
        }
    }
}

void InitGameStart::render(sf::RenderTarget& target) const {
    if (isRendering) {
        target.draw(this->buttonBackground);
        this->chooseKingdomButton->draw(target);
        this->playButton->draw(target);
        this->returnButton->draw(target);
    }
}

void InitGameStart::startRender() {
    this->isRendering = true;
}

void InitGameStart::startUpdate() {
    this->isUpdating = true;
}
