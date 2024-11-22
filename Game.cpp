#include "pch.h"

void Game::initVariables() {
    this->window = nullptr;
    this->videoMode = sf::VideoMode::getDesktopMode();
    this->dt = 0.f;
    this->opacity = 0.f;
    this->fadeDirection = 1; // 1 for fade-in, -1 for fade-out
    this->loadFinished = false;
    this->playOnce = true;
}

void Game::initLoadingText() {
    if (!this->font.loadFromFile("Fonts/metropolis/Metropolis-SemiBold.otf")) {
        std::cout << "ERROR::GAME::INIT_LOADING_TEXT::Failed to load font" << std::endl;
    }

    this->loadingTextName.setFont(this->font);
    this->loadingTextPresents.setFont(this->font);
    this->loadingTextName.setCharacterSize(200);
    this->loadingTextPresents.setCharacterSize(200);
    this->loadingTextName.setFillColor(sf::Color(255, 0, 0, 0)); // Initial alpha set to 0
    this->loadingTextPresents.setFillColor(sf::Color(255, 0, 0, 0)); // Initial alpha set to 0
    this->loadingTextName.setString("Nikoloz Taturashvili");
    this->loadingTextPresents.setString("Presents");
    this->loadingTextName.setPosition(
        static_cast<float>(this->videoMode.width) / 2.f - this->loadingTextName.getGlobalBounds().width / 2.f,
        static_cast<float>(this->videoMode.height) / 2.f - this->loadingTextName.getGlobalBounds().height / 2.f - 150
    );
    this->loadingTextPresents.setPosition(
        static_cast<float>(this->videoMode.width) / 2.f - this->loadingTextPresents.getGlobalBounds().width / 2.f,
        static_cast<float>(this->videoMode.height) / 2.f - this->loadingTextPresents.getGlobalBounds().height / 2.f + 150
    );

    // Start the clock for fading effect
    this->fadeClock.restart();
}

void Game::initWindow() {
    this->window = new sf::RenderWindow(
        this->videoMode,
        "Kingdom Builder",
        sf::Style::Fullscreen
    );
    this->window->setFramerateLimit(60);
}

void Game::pollEvents() {
    if (mainMenu.getEndGame()) {
        this->window->close();
    } else if (initGameStart.getEndGame()) {
        this->window->close();
    }
    while (this->window->pollEvent(this->ev)) {
        initGameStart.setEvt(ev);
        if (initGameStart.getPlayGame()) {
            this->inGame->update(this->ev);
        }
        switch (this->ev.type) {
        case sf::Event::Closed:
            this->window->close();
            break;
        default: if (fadeClock.getElapsedTime().asSeconds() <= 10.f && this->ev.key.code == sf::Keyboard::Space) {
            this->loadFinished = true;
            this->playMusic();
        } break;
        }
    }
}

void Game::playMusic() {
    if (this->playOnce) {
        backgroundMusic.play();
        this->playOnce = false;
    }
}

void Game::updateLoadingText() {
    if (const float elapsedTime = fadeClock.getElapsedTime().asSeconds(); elapsedTime <= 1.5f) {
        // Initial wait for 1.5 seconds, keep opacity at 0
        opacity = 0;
    }
    else if (elapsedTime <= 3.0f) {
        // Fade-in for the next 1.5 seconds (1.5s to 3.0s)
        opacity = 255 * ((elapsedTime - 1.5f) / 1.5f); // Gradually increase alpha to 255
    }
    else if (elapsedTime <= 6.0f) {
        // Full opacity for the next 3 seconds (3.0s to 6.0s)
        opacity = 255;
    }
    else if (elapsedTime <= 7.5f) {
        // Fade-out for the last 1.5 seconds (6.0s to 7.5s)
        opacity = 255 * (1.0f - (elapsedTime - 6.0f) / 1.5f); // Gradually decrease alpha to 0
    }
    else if (elapsedTime <= 10.f) {
        // After 10 seconds, hide text completely
        opacity = 0;
    }
    else {
        this->loadFinished = true;
        this->playMusic();
    }

    // Update text color with the new opacity
    this->loadingTextName.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(opacity)));
    this->loadingTextPresents.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(opacity)));
}

void Game::updateMainMenu() const {
    if (loadFinished) {
        mainMenu.update();
    }
}

void Game::updateInitGameStart() {
    if (mainMenu.getGameStart()) {
        initGameStart.update();
    }
}


void Game::update() {
    this->pollEvents();
    this->updateLoadingText();
    this->updateMainMenu();
    this->updateInitGameStart();

    if (initGameStart.getPlayGame()) {
        // If you have any additional update logic for InGame that's not event-based,
        // you can add it here
    }
}

void Game::renderLoadingText(sf::RenderTarget &target) const {
        target.draw(this->loadingTextName);
        target.draw(this->loadingTextPresents);
}

void Game::renderMainMenu(sf::RenderTarget &target) const {
    this->mainMenu.render(target);
}

void Game::render() const {
    this->window->clear();

    if (!loadFinished) {
        this->renderLoadingText(*this->window);
    }

    if (loadFinished) {
        this->renderMainMenu(*this->window);
    }

    if (mainMenu.getGameStart()) {
        this->initGameStart.render(*this->window);
    }

    if (initGameStart.getReturnButtonClicked()) {
        mainMenu.setReturnButtonClicked(false);
        initGameStart.setReturnButtonClicked(false);
        mainMenu.setGameStart(false);
    }

    if (initGameStart.getPlayGame()) {
        initGameStart.stopRender();
        initGameStart.stopUpdate();
        mainMenu.stopRender();
        mainMenu.stopUpdate();
    }

    if (initGameStart.getPlayGame()) {
        inGame->render();
        inGame->initUserKingdomName(initGameStart.userKingdomName);
    }

    this->window->display();
}

Game::Game(MainMenu& mainMenu, InitGameStart& initGameStart) : mainMenu(mainMenu), initGameStart(initGameStart), inGame(nullptr) {
    this->initVariables();
    this->initWindow();
    this->initLoadingText();
    this->initMusic();
    this->inGame = new InGame(this->window);
    this->inGame->loadTexture("Textures/game_map.jpg");
}

Game::~Game() {
    backgroundMusic.stop();
    delete this->inGame;
    delete this->window;
}

void Game::initMusic() {
    // Load the music file
    if (!backgroundMusic.openFromFile("Effects/background_music.wav")) {
        // Error handling if music file can't be loaded
        std::cout << "ERROR::GAME::INIT_MUSIC::Failed to load background music" << std::endl;
        return;
    }

    // Set the music to loop
    backgroundMusic.setLoop(true);

    // Set volume (optional, adjust as needed)
    // Volume ranges from 0 to 100
    backgroundMusic.setVolume(10.f);

}

void Game::resumeMusic() {
    backgroundMusic.play();
}

void Game::stopMusic() {
    backgroundMusic.stop();
}

sf::Event & Game::getEv() {
    return this->ev;
}

void Game::run() {
    while (this->window->isOpen()) {
        dt = dtClock.restart().asSeconds();

        this->update();
        this->render();
    }
}
