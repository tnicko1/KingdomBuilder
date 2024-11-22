#include "pch.h"

void MainMenu::initVariables() {
    this->videoMode = sf::VideoMode::getDesktopMode();
    this->font.loadFromFile("Fonts/metropolis/Metropolis-SemiBold.otf");
    this->endGame = false;
    this->startGameBool = false;
    this->returnButtonClicked = false;
    this->isUpdating = true;
    this->isRendering = true;
    this->showHowToPlay = false;
    this->currentTutorialPage = 0;
    this->cooldownTime = 0.2f; // Set cooldown to 200ms
    this->canClick = true;

    this->showCredits = false;

    // Initialize credits text
    this->creditsText.setFont(this->font);
    this->creditsText.setCharacterSize(48);
    this->creditsText.setFillColor(sf::Color::White);
    this->creditsText.setPosition(
        static_cast<float>(videoMode.width) / 8.f,
        static_cast<float>(videoMode.height) / 2.f - 150.f
    );
}

void MainMenu::initHowToPlayButtons() {
    // Next button
    this->nextButton = new Button(
        sf::Vector2f(static_cast<float>(videoMode.width) - 200.f, static_cast<float>(videoMode.height) - 100.f),
        sf::Vector2f(150.f, 50.f),
        "Next",
        this->font,
        sf::Color(0, 0, 0, 150),
        sf::Color(0, 0, 0, 200)
    );

    // Previous button
    this->prevButton = new Button(
        sf::Vector2f(50.f, static_cast<float>(videoMode.height) - 100.f),
        sf::Vector2f(150.f, 50.f),
        "Previous",
        this->font,
        sf::Color(0, 0, 0, 150),
        sf::Color(0, 0, 0, 200)
    );

    // Back button
    this->backButton = new Button(
        sf::Vector2f(50.f, 50.f),
        sf::Vector2f(150.f, 50.f),
        "Back",
        this->font,
        sf::Color(0, 0, 0, 150),
        sf::Color(0, 0, 0, 200)
    );
}

void MainMenu::initCreditsButton() {
    // Create return button for credits screen
    this->creditsReturnButton = new Button(
        sf::Vector2f(50.f, 50.f),
        sf::Vector2f(150.f, 50.f),
        "Back",
        this->font,
        sf::Color(0, 0, 0, 150),
        sf::Color(0, 0, 0, 200)
    );

    setCreditsText("Hello, my name is Nikoloz Taturashvili and I am a student at Tbilisi State University.\nThis game was created for an assignment for the Data Structures(C++) course.\nThe game is a simple risk type kingdom game.\nThe game is nowhere near complete\nAnd I am not planning to add more features in the future. ;)\nI hope you enjoy the game! Thank you for playing!\nSpecial thanks to the creators of the SFML library for making this game possible");
}

void MainMenu::setCreditsText(const std::string& credits) {
    this->creditsText.setString(credits);
}

void MainMenu::initBackground() {
    if (!this->mainMenuTexture.loadFromFile("Textures/main_menu_background.jpg")) {
        std::cout << "ERROR::GAME::INIT_MAIN_MENU::Failed to load main menu texture" << std::endl;
    }

    this->mainMenuSprite.setTexture(this->mainMenuTexture);
    this->mainMenuSprite.setScale(
        static_cast<float>(this->videoMode.width) / this->mainMenuSprite.getGlobalBounds().width,
        static_cast<float>(this->videoMode.height) / this->mainMenuSprite.getGlobalBounds().height
    );

    this->buttonBackground.setSize(sf::Vector2f(static_cast<float>(videoMode.width) / 4.f, static_cast<float>(videoMode.height)));
    this->buttonBackground.setFillColor(sf::Color(0, 0, 0, 150));
    this->buttonBackground.setPosition(0, 0);

    if (!this->titleTexture.loadFromFile("Textures/kingdom_builder_logo.png")) {
        std::cout << "ERROR::GAME::INIT_MAIN_MENU::Failed to load main menu texture" << std::endl;
    }
    this->titleSprite.setTexture(this->titleTexture);
    this->titleSprite.setScale(0.9f, 0.9f);
    this->titleSprite.setPosition(
        this->buttonBackground.getGlobalBounds().width / 2.f - this->titleSprite.getGlobalBounds().width / 2.f,
        -this->titleSprite.getGlobalBounds().height / 4.f
        );
}

void MainMenu::initButtons() {
    this->startButton = new Button(
        sf::Vector2f(0.f, this->buttonBackground.getLocalBounds().height / 2.f - 100.f),
        sf::Vector2f(this->buttonBackground.getGlobalBounds().width, 50.f),
        "Start",
        this->font,
        sf::Color(0, 0, 0, 0),
        sf::Color(0, 0, 0, 150)
    );

    this->howToPlayButton = new Button(
        sf::Vector2f(0.f, this->buttonBackground.getLocalBounds().height / 2.f - 50.f),
        sf::Vector2f(this->buttonBackground.getGlobalBounds().width, 50.f),
        "How to Play",
        this->font,
        sf::Color(0, 0, 0, 0),
        sf::Color(0, 0, 0, 150)
    );

    this->creditsButton = new Button(
        sf::Vector2f(0.f, this->buttonBackground.getLocalBounds().height / 2.f + 50.f),
        sf::Vector2f(this->buttonBackground.getGlobalBounds().width, 50.f),
        "Credits",
        this->font,
        sf::Color(0, 0, 0, 0),
        sf::Color(0, 0, 0, 150)
    );

    this->exitButton = new Button(
        sf::Vector2f(0.f, this->buttonBackground.getLocalBounds().height / 2.f + 100.f),
        sf::Vector2f(this->buttonBackground.getGlobalBounds().width, 50.f),
        "Exit",
        this->font,
        sf::Color(0, 0, 0, 0),
        sf::Color(0, 0, 0, 150)
    );
}

void MainMenu::startGame() {
    this->startGameBool = true;
}

void MainMenu::deleteButtons() const {
    delete this->startButton;
    delete this->howToPlayButton;
    delete this->creditsButton;
    delete this->exitButton;
}

MainMenu::MainMenu() {
    this->initVariables();
    this->initBackground();
    this->initButtons();
    this->initHowToPlayButtons();
    this->initCreditsButton();
}

MainMenu::~MainMenu() {
    this->deleteButtons();
    delete this->nextButton;
    delete this->prevButton;
    delete this->backButton;
    delete this->creditsReturnButton;
}

bool MainMenu::getEndGame() const {
    return this->endGame;
}

bool MainMenu::getGameStart() const {
    return this->startGameBool;
}

void MainMenu::setGameStart(const bool start) {
    this->startGameBool = start;
}

void MainMenu::setReturnButtonClicked(const bool clicked) {
    this->returnButtonClicked = clicked;
}

void MainMenu::loadTutorialImages(const std::vector<std::string>& imagePaths) {
    tutorialTextures.clear();
    for (const auto& path : imagePaths) {
        sf::Texture texture;
        if (!texture.loadFromFile(path)) {
            std::cout << "ERROR::GAME::LOAD_TUTORIAL::Failed to load tutorial image: " << path << std::endl;
            continue;
        }
        tutorialTextures.push_back(texture);
    }

    if (!tutorialTextures.empty()) {
        tutorialSprite.setTexture(tutorialTextures[0]);
        // Scale to fit screen while maintaining aspect ratio
        float scaleX = (static_cast<float>(videoMode.width) * 0.8f) / tutorialSprite.getGlobalBounds().width;
        float scaleY = (static_cast<float>(videoMode.height) * 0.8f) / tutorialSprite.getGlobalBounds().height;
        float scale = std::min(scaleX, scaleY);
        tutorialSprite.setScale(scale, scale);

        // Center the sprite
        tutorialSprite.setPosition(
            (videoMode.width - tutorialSprite.getGlobalBounds().width) / 2.f,
            (videoMode.height - tutorialSprite.getGlobalBounds().height) / 2.f
        );
    }
}

void MainMenu::updateTutorialPage() {
    if (currentTutorialPage < tutorialTextures.size()) {
        tutorialSprite.setTexture(tutorialTextures[currentTutorialPage]);
        // Maintain the same scaling and positioning
        float scaleX = (static_cast<float>(videoMode.width) * 0.8f) / tutorialSprite.getGlobalBounds().width;
        float scaleY = (static_cast<float>(videoMode.height) * 0.8f) / tutorialSprite.getGlobalBounds().height;
        float scale = std::min(scaleX, scaleY);
        tutorialSprite.setScale(scale, scale);

        tutorialSprite.setPosition(
            (videoMode.width - tutorialSprite.getGlobalBounds().width) / 2.f,
            (videoMode.height - tutorialSprite.getGlobalBounds().height) / 2.f
        );
    }
}

void MainMenu::stopUpdate() {
    this->isUpdating = false;
}

void MainMenu::stopRender() {
    this->isRendering = false;
}

//Functions
void MainMenu::update() {
    if (isUpdating) {
        const sf::Vector2i mousePos = sf::Mouse::getPosition();

        // Update click cooldown
        if (!canClick) {
            if (buttonCooldown.getElapsedTime().asSeconds() >= cooldownTime) {
                canClick = true;
            }
        }

        // If credits screen is shown
        if (showCredits) {
            this->creditsReturnButton->update(mousePos);

            if (canClick) {
                if (this->creditsReturnButton->isClicked(mousePos)) {
                    showCredits = false;
                    canClick = false;
                    buttonCooldown.restart();
                }
            }
            return;
        }

        if (showHowToPlay) {
            // Update tutorial navigation buttons
            this->nextButton->update(mousePos);
            this->prevButton->update(mousePos);
            this->backButton->update(mousePos);

            if (canClick) {
                if (this->nextButton->isClicked(mousePos) && currentTutorialPage < tutorialTextures.size() - 1) {
                    currentTutorialPage++;
                    updateTutorialPage();
                    canClick = false;
                    buttonCooldown.restart();
                }

                if (this->prevButton->isClicked(mousePos) && currentTutorialPage > 0) {
                    currentTutorialPage--;
                    updateTutorialPage();
                    canClick = false;
                    buttonCooldown.restart();
                }

                if (this->backButton->isClicked(mousePos)) {
                    showHowToPlay = false;
                    currentTutorialPage = 0;
                    canClick = false;
                    buttonCooldown.restart();
                }
            }
        }
        else {
            this->startButton->update(mousePos);
            this->howToPlayButton->update(mousePos);
            this->creditsButton->update(mousePos);
            this->exitButton->update(mousePos);

            if (canClick) {
                if (this->startButton->isClicked(mousePos) && !startGameBool) {
                    this->startGame();
                    this->returnButtonClicked = true;
                    canClick = false;
                    buttonCooldown.restart();
                }

                if (this->howToPlayButton->isClicked(mousePos)) {
                    showHowToPlay = true;
                    currentTutorialPage = 0;
                    updateTutorialPage();
                    canClick = false;
                    buttonCooldown.restart();
                }

                if (this->creditsButton->isClicked(mousePos)) {
                    showCredits = true;
                    canClick = false;
                    buttonCooldown.restart();
                }

                if (this->exitButton->isClicked(mousePos)) {
                    this->endGame = true;
                    canClick = false;
                    buttonCooldown.restart();
                }
            }
        }
    }
}

void MainMenu::setCooldownTime(float seconds) {
    this->cooldownTime = seconds;
}

void MainMenu::render(sf::RenderTarget& target) const {
    if (isRendering) {
        target.draw(this->mainMenuSprite);

        if (showCredits) {
            // Draw semi-transparent background
            sf::RectangleShape background;
            background.setSize(sf::Vector2f(static_cast<float>(videoMode.width), static_cast<float>(videoMode.height)));
            background.setFillColor(sf::Color(0, 0, 0, 200));
            target.draw(background);

            // Draw credits text
            target.draw(this->creditsText);

            // Draw return button
            this->creditsReturnButton->draw(target);
        }
        else if (showHowToPlay) {
            // Existing how to play rendering
            target.draw(tutorialSprite);
            this->nextButton->draw(target);
            this->prevButton->draw(target);
            this->backButton->draw(target);
        }
        else if (!startGameBool && !returnButtonClicked) {
            // Existing main menu rendering
            target.draw(this->buttonBackground);
            target.draw(this->titleSprite);
            this->startButton->draw(target);
            this->howToPlayButton->draw(target);
            this->creditsButton->draw(target);
            this->exitButton->draw(target);
        }
    }
}

void MainMenu::startRender() {
    this->isRendering = true;
}

void MainMenu::startUpdate() {
    this->isUpdating = true;
}
