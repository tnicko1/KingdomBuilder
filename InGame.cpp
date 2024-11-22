#include "pch.h"
#include "InGame.h"
#include "PauseMenu.h"
#include <sstream>

InGame::InGame(sf::RenderWindow* window) : window(window) {
    baseResolution = sf::Vector2f(2560.f, 1440.f); // Your development resolution
    initVariables();
    initText();
    initChooseKingdom();
    calculateScaleFactor();
    isDiceResultShowing = false;
    isPaused = false;
    pauseMenu = new PauseMenu(window, &isPaused);
    this->firstClick = true;
    this->kingdomChosen = false;
    this->previousClickedText = nullptr;
    this->battleWon = false;
    this->playerKingdom = nullptr;
    loadDiceTextures();

    // Initialize the color map with original colors
    currentTerritoryColors[&elvenIslesTitle] = sf::Color(177, 115, 206, 255);
    currentTerritoryColors[&iceReachTitle] = sf::Color(191, 191, 191, 255);
    currentTerritoryColors[&thrymholdTitle] = sf::Color(77, 100, 141, 255);
    currentTerritoryColors[&solmareTitle] = sf::Color(248, 110, 81, 255);
    currentTerritoryColors[&lowenvaleTitle] = sf::Color(244, 215, 96, 255);
    currentTerritoryColors[&embercrownTitle] = sf::Color(189, 126, 74, 255);
    currentTerritoryColors[&everdellTitle] = sf::Color(72, 185, 159, 255);
    currentTerritoryColors[&concordiaTitle] = sf::Color(255, 137, 255, 255);

    isVictory = false;
    isGameOver = false;

    // Initialize victory text
    victoryText.setFont(battleFont);
    victoryText.setString("Victory");
    victoryText.setCharacterSize(72);
    victoryText.setFillColor(sf::Color::White);
    victoryText.setStyle(sf::Text::Bold);

    // Initialize semi-transparent black background for victory screen
    victoryBackground.setFillColor(sf::Color(0, 0, 0, 200));
    victoryBackground.setSize(sf::Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
}

InGame::~InGame() {
    // Currently empty as SFML handles cleanup of its own resources
    // Clean up tracked sprites
    delete pauseMenu;
    delete hoveredText;
    delete clickedText;
}

void InGame::calculateScaleFactor() {
    sf::Vector2u windowSize = window->getSize();
    float scaleX = windowSize.x / baseResolution.x;
    float scaleY = windowSize.y / baseResolution.y;
    scaleFactor = std::min(scaleX, scaleY); // Use the smaller scale to maintain aspect ratio
}

void InGame::updatePositions() {
    // Update sprite positions and scales
    mapSprite.setScale(BASE_SCALE * scaleFactor, BASE_SCALE * scaleFactor);

    // Update all territory sprites with the new scale
    float territoryScale = BASE_SCALE * scaleFactor;

    elvenIslesSprite.setScale(territoryScale, territoryScale);
    elvenIslesSprite.setPosition(1954.f * scaleFactor, 586.f * scaleFactor);

    iceReachSprite.setScale(territoryScale, territoryScale);
    iceReachSprite.setPosition(240.f * scaleFactor, 165.f * scaleFactor);

    thrymholdSprite.setScale(territoryScale, territoryScale);
    thrymholdSprite.setPosition(980.f * scaleFactor, 42.f * scaleFactor);

    solmareSprite.setScale(territoryScale, territoryScale);
    solmareSprite.setPosition(1178.f * scaleFactor, 397.f * scaleFactor);

    lowenvaleSprite.setScale(territoryScale, territoryScale);
    lowenvaleSprite.setPosition(1050.f * scaleFactor, 382.f * scaleFactor);

    embercrownSprite.setScale(territoryScale, territoryScale);
    embercrownSprite.setPosition(298.f * scaleFactor, 789.f * scaleFactor);

    everdellSprite.setScale(territoryScale, territoryScale);
    everdellSprite.setPosition(295.f * scaleFactor, 532.f * scaleFactor);

    concordiaSprite.setScale(territoryScale, territoryScale);
    concordiaSprite.setPosition(575.f * scaleFactor, 412.f * scaleFactor);

    // Update text positions and sizes
    float textSize = 42.f * scaleFactor;

    elvenIslesTitle.setCharacterSize(static_cast<unsigned int>(textSize));
    elvenIslesTitle.setPosition(2050.f * scaleFactor, 850.f * scaleFactor);

    iceReachTitle.setCharacterSize(static_cast<unsigned int>(textSize));
    iceReachTitle.setPosition(600.f * scaleFactor, 300.f * scaleFactor);

    thrymholdTitle.setCharacterSize(static_cast<unsigned int>(textSize));
    thrymholdTitle.setPosition(1100.f * scaleFactor, 300.f * scaleFactor);

    solmareTitle.setCharacterSize(static_cast<unsigned int>(textSize));
    solmareTitle.setPosition(1500.f * scaleFactor, 850.f * scaleFactor);

    lowenvaleTitle.setCharacterSize(static_cast<unsigned int>(textSize));
    lowenvaleTitle.setPosition(1200.f * scaleFactor, 550.f * scaleFactor);

    embercrownTitle.setCharacterSize(static_cast<unsigned int>(textSize));
    embercrownTitle.setPosition(600.f * scaleFactor, 1025.f * scaleFactor);

    everdellTitle.setCharacterSize(static_cast<unsigned int>(textSize));
    everdellTitle.setPosition(500.f * scaleFactor, 700.f * scaleFactor);

    concordiaTitle.setCharacterSize(static_cast<unsigned int>(textSize));
    concordiaTitle.setPosition(850.f * scaleFactor, 700.f * scaleFactor);

    // Update other UI elements
    chooseKingdomBackground.setSize(sf::Vector2f(static_cast<float>(window->getSize().x), 100.f * scaleFactor));
    chooseKingdomBackground.setPosition(0.f, static_cast<float>(window->getSize().y) - 100.f * scaleFactor);

    chooseKingdomText.setCharacterSize(static_cast<unsigned int>(42.f * scaleFactor));
    chooseKingdomText.setPosition(
        window->getSize().x / 2.f - chooseKingdomText.getGlobalBounds().width / 2.f,
        static_cast<float>(window->getSize().y) - 80.f * scaleFactor
    );

    // Update battle-related elements
    battleResult.setCharacterSize(static_cast<unsigned int>(32.f * scaleFactor));
    victoryText.setCharacterSize(static_cast<unsigned int>(72.f * scaleFactor));
    gameOverText.setCharacterSize(static_cast<unsigned int>(72.f * scaleFactor));
    exitButtonText.setCharacterSize(static_cast<unsigned int>(32.f * scaleFactor));
}

void InGame::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        isPaused = !isPaused;
    }
    else if (event.type == sf::Event::Resized) {
        // Update the view
        gameView.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
        gameView.setCenter(static_cast<float>(event.size.width) / 2.0f, static_cast<float>(event.size.height) / 2.0f);

        // Recalculate scaling and update positions
        calculateScaleFactor();
        updatePositions();
    }
}

// Update getOriginalColor to getCurrentColor
sf::Color InGame::getCurrentColor(sf::Text* text) {
    auto it = currentTerritoryColors.find(text);
    if (it != currentTerritoryColors.end()) {
        return it->second;
    }
    return sf::Color::White;  // Default color if not found
}

sf::Sprite& InGame::getAssociatedSprite(sf::Text* text) {
    if (text == &elvenIslesTitle) return elvenIslesSprite;
    if (text == &iceReachTitle) return iceReachSprite;
    if (text == &thrymholdTitle) return thrymholdSprite;
    if (text == &solmareTitle) return solmareSprite;
    if (text == &lowenvaleTitle) return lowenvaleSprite;
    if (text == &embercrownTitle) return embercrownSprite;
    if (text == &everdellTitle) return everdellSprite;
    if (text == &concordiaTitle) return concordiaSprite;
    return mapSprite; // Default case, should never happen
}

void InGame::loadDiceTextures() {
    diceTextures.resize(6);
    for (int i = 1; i <= 6; i++) {
        sf::Texture texture;
        if (!texture.loadFromFile("Textures/dice" + std::to_string(i) + ".png")) {
            std::cout << "Error loading dice texture " << i << std::endl;
        }
        diceTextures[i-1] = texture;
    }

    // Initialize both dice sprites
    attackerDiceSprite.setScale(0.5f, 0.5f);
    defenderDiceSprite.setScale(0.5f, 0.5f);

    isDiceAnimating = false;
    currentDiceFrame = 0;
}

void InGame::updateDiceAnimation() {
    if (isDiceAnimating) {
        if (diceAnimationClock.getElapsedTime().asSeconds() >= DICE_ANIMATION_FRAME_DURATION) {
            currentDiceFrame++;

            if (currentDiceFrame >= TOTAL_DICE_FRAMES) {
                int attackerRoll = rand() % 6 + 1;
                int defenderRoll = rand() % 6 + 1;

                attackerDiceSprite.setTexture(diceTextures[attackerRoll - 1]);
                defenderDiceSprite.setTexture(diceTextures[defenderRoll - 1]);

                battleWon = attackerRoll > defenderRoll;

                std::stringstream ss;
                ss << "You " << defenderRoll << " vs Enemy " << attackerRoll;
                battleResultText = ss.str();
                battleResult.setString(battleResultText);

                sf::FloatRect textBounds = battleResult.getLocalBounds();
                battleResult.setPosition(
                    (attackerDicePosition.x + defenderDicePosition.x) / 2.f - textBounds.width / 2.f,
                    attackerDicePosition.y - 50.f
                );

                resultDisplayClock.restart();
                isDiceResultShowing = true;

                if (previousClickedText != nullptr && clickedText != nullptr) {
                    sf::Sprite& previousSprite = getAssociatedSprite(previousClickedText);
                    sf::Sprite& currentSprite = getAssociatedSprite(clickedText);

                    if (attackerRoll == defenderRoll) {
                        battleWon = (rand() % 100) < 40;
                    }

                    if (battleWon) {
                        // Current clicked territory gets conquered
                        previousSprite.setColor(currentClickedColor);
                        currentTerritoryColors[previousClickedText] = currentClickedColor;
                        // The conquered territory becomes part of the player's kingdom
                        if (currentClickedColor == playerKingdomColor) {
                            currentSprite.setColor(playerKingdomColor);
                            currentTerritoryColors[clickedText] = playerKingdomColor;
                        }
                    } else {
                        // Previous territory wins
                        currentSprite.setColor(previousClickedColor);
                        currentTerritoryColors[clickedText] = previousClickedColor;
                    }
                }

                isDiceAnimating = false;
                currentDiceFrame = 0;

                previousClickedText = nullptr;
                clickedText = nullptr;
                isPressed = false;

                isGameOver = checkGameOver();
                if (isGameOver) {
                    // Center game over text and exit button
                    sf::Vector2u windowSize = window->getSize();
                    gameOverText.setPosition(
                        static_cast<float>(windowSize.x) / 2.f - gameOverText.getGlobalBounds().width / 2.f,
                        static_cast<float>(windowSize.y) / 2.f - gameOverText.getGlobalBounds().height / 2.f - 50.f
                    );

                    exitButton.setPosition(
                        static_cast<float>(windowSize.x) / 2.f - exitButton.getSize().x / 2.f,
                        static_cast<float>(windowSize.y) / 2.f + 50.f
                    );

                    exitButtonText.setPosition(
                        static_cast<float>(windowSize.x) / 2.f - exitButtonText.getGlobalBounds().width / 2.f,
                        static_cast<float>(windowSize.y) / 2.f + 50.f + (exitButton.getSize().y - exitButtonText.getGlobalBounds().height) / 2.f
                    );
                }
            } else {
                attackerDiceSprite.setTexture(diceTextures[rand() % 6]);
                defenderDiceSprite.setTexture(diceTextures[rand() % 6]);
            }
            diceAnimationClock.restart();
        }
    } else if (isDiceResultShowing) {
        if (resultDisplayClock.getElapsedTime().asSeconds() >= BATTLE_RESULT_DISPLAY_DURATION) {
            isDiceResultShowing = false;
        }
    }
}


void InGame::startDiceAnimation(const sf::Vector2f& position) {
    isDiceAnimating = true;
    currentDiceFrame = 0;

    // Calculate positions for both dice
    float diceSpacing = 100.0f; // Adjust this value to change spacing between dice
    attackerDicePosition = sf::Vector2f(position.x - diceSpacing/2, position.y);
    defenderDicePosition = sf::Vector2f(position.x + diceSpacing/2, position.y);

    attackerDiceSprite.setPosition(defenderDicePosition);
    defenderDiceSprite.setPosition(attackerDicePosition);

    attackerDiceSprite.setTexture(diceTextures[0]);
    defenderDiceSprite.setTexture(diceTextures[0]);

    diceAnimationClock.restart();
}

void InGame::renderDiceAnimation() const {
    if (isDiceAnimating || isDiceResultShowing) {
        window->draw(attackerDiceSprite);
        window->draw(defenderDiceSprite);
        if (isDiceResultShowing) {
            window->draw(battleResult);
        }
    }
}

void InGame::initVariables() {
    // Initialize view with the window size
    gameView.setSize(static_cast<float>(window->getSize().x),
                     static_cast<float>(window->getSize().y));
    gameView.setCenter(static_cast<float>(window->getSize().x) / 2.0f,
                      static_cast<float>(window->getSize().y) / 2.0f);

    hoveredText = nullptr;
    clickedText = nullptr;

    if (!hoverBuffer.loadFromFile("Effects/sprite-hover.wav")) {
        std::cout << "Error loading hover sound!" << std::endl;
    }
    hoverSound.setBuffer(hoverBuffer);
    hoverSound.setVolume(30);  // Default volume

    wasHovered = false;
    isPressed = false;

    this->font.loadFromFile("Fonts/ringbearer/RingbearerMedium.ttf");
    this->battleFont.loadFromFile("Fonts/metropolis/Metropolis-SemiBold.otf");

    battleResult.setFont(battleFont);
    battleResult.setCharacterSize(32);
    battleResult.setFillColor(sf::Color::White);
    battleResult.setOutlineColor(sf::Color::Black);
    battleResult.setOutlineThickness(2.0f);

    isGameOver = false;

    // Initialize game over text
    gameOverText.setFont(battleFont);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(72);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setStyle(sf::Text::Bold);

    // Initialize semi-transparent black background
    gameOverBackground.setFillColor(sf::Color(0, 0, 0, 200));
    gameOverBackground.setSize(sf::Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));

    // Initialize exit button
    exitButton.setSize(sf::Vector2f(200.f, 100.f));
    exitButton.setFillColor(sf::Color(0, 0, 0, 0));

    exitButtonText.setFont(battleFont);
    exitButtonText.setString("Exit");
    exitButtonText.setCharacterSize(32);
    exitButtonText.setFillColor(sf::Color::White);
}

void InGame::initText() {
    this->elvenIslesTitle.setFont(font);
    this->elvenIslesTitle.setString("Elven Isles");
    this->elvenIslesTitle.setCharacterSize(42);
    this->elvenIslesTitle.setFillColor(sf::Color::White);
    this->elvenIslesTitle.setPosition(2050.f, 850.f);

    this->iceReachTitle.setFont(font);
    this->iceReachTitle.setString("Ice Reach");
    this->iceReachTitle.setCharacterSize(42);
    this->iceReachTitle.setFillColor(sf::Color::White);
    this->iceReachTitle.setPosition(600.f, 300.f);

    this->thrymholdTitle.setFont(font);
    this->thrymholdTitle.setString("Thrymhold");
    this->thrymholdTitle.setCharacterSize(42);
    this->thrymholdTitle.setFillColor(sf::Color::White);
    this->thrymholdTitle.setPosition(1100.f, 300.f);

    this->solmareTitle.setFont(font);
    this->solmareTitle.setString("Solmare");
    this->solmareTitle.setCharacterSize(42);
    this->solmareTitle.setFillColor(sf::Color::White);
    this->solmareTitle.setPosition(1500.f, 850.f);

    this->lowenvaleTitle.setFont(font);
    this->lowenvaleTitle.setString("Lowenvale");
    this->lowenvaleTitle.setCharacterSize(42);
    this->lowenvaleTitle.setFillColor(sf::Color::White);
    this->lowenvaleTitle.setPosition(1200.f, 550.f);

    this->embercrownTitle.setFont(font);
    this->embercrownTitle.setString("Embercrown");
    this->embercrownTitle.setCharacterSize(42);
    this->embercrownTitle.setFillColor(sf::Color::White);
    this->embercrownTitle.setPosition(600.f, 1025.f);

    this->everdellTitle.setFont(font);
    this->everdellTitle.setString("Everdell");
    this->everdellTitle.setCharacterSize(42);
    this->everdellTitle.setFillColor(sf::Color::White);
    this->everdellTitle.setPosition(500.f, 700.f);

    this->concordiaTitle.setFont(font);
    this->concordiaTitle.setString("Concordia");
    this->concordiaTitle.setCharacterSize(42);
    this->concordiaTitle.setFillColor(sf::Color::White);
    this->concordiaTitle.setPosition(850.f, 700.f);
}

void InGame::initChooseKingdom() {
    this->chooseKingdomBackground.setSize(sf::Vector2f(static_cast<float>(window->getSize().x), 100.f));
    this->chooseKingdomBackground.setFillColor(sf::Color(0, 0, 0, 150));
    this->chooseKingdomBackground.setPosition(0.f, static_cast<float>(window->getSize().y) - 100.f);

    this->chooseKingdomText.setFont(font);
    this->chooseKingdomText.setString("Choose a Kingdom");
    this->chooseKingdomText.setCharacterSize(42);
    this->chooseKingdomText.setFillColor(sf::Color::White);
    this->chooseKingdomText.setPosition(chooseKingdomBackground.getGlobalBounds().width / 2.f - chooseKingdomText.getGlobalBounds().width / 2.f, static_cast<float>(window->getSize().y) - 80.f);
}

bool InGame::checkGameOver() {
    if (!kingdomChosen) return false;  // Game hasn't started yet

    bool playerTerritoriesExist = false;
    bool otherTerritoriesExist = false;

    // Check all territories
    for (const auto& pair : currentTerritoryColors) {
        if (pair.second == playerKingdomColor) {
            playerTerritoriesExist = true;
        } else {
            otherTerritoriesExist = true;
        }
    }

    // Check victory condition
    if (playerTerritoriesExist && !otherTerritoriesExist) {
        isVictory = true;
        return false; // Not game over, but victory
    }

    // Check defeat condition
    return !playerTerritoriesExist;
}

void InGame::loadTexture(const std::string& texturePath) {
    if (!mapTexture.loadFromFile(texturePath)) {
        throw std::runtime_error("Failed to load map texture: " + texturePath);
    }

    mapSprite.setTexture(mapTexture);
    mapSprite.setScale(BASE_SCALE, BASE_SCALE);

    elvenIslesTexture.loadFromFile("Textures/elven_isles.png");
    elvenIslesSprite.setTexture(elvenIslesTexture);
    elvenIslesSprite.setPosition(1954.f, 586.f);
    elvenIslesSprite.setScale(1.35f, 1.35f);
    elvenIslesSprite.setColor(sf::Color(177, 115, 206, 255));

    iceReachTexture.loadFromFile("Textures/icereach.png");
    iceReachSprite.setTexture(iceReachTexture);
    iceReachSprite.setPosition(240.f, 165.f);
    iceReachSprite.setScale(1.35f, 1.35f);
    iceReachSprite.setColor(sf::Color(191, 191, 191, 255));

    thrymholdTexture.loadFromFile("Textures/thrymhold.png");
    thrymholdSprite.setTexture(thrymholdTexture);
    thrymholdSprite.setPosition(980.f, 42.f);
    thrymholdSprite.setScale(1.35f, 1.35f);
    thrymholdSprite.setColor(sf::Color(77, 100, 141, 255));

    solmareTexture.loadFromFile("Textures/solmare.png");
    solmareSprite.setTexture(solmareTexture);
    solmareSprite.setPosition(1178.f, 397.f);
    solmareSprite.setScale(1.35f, 1.35f);
    solmareSprite.setColor(sf::Color(248, 110, 81, 255));

    lowenvaleTexture.loadFromFile("Textures/lowenvale.png");
    lowenvaleSprite.setTexture(lowenvaleTexture);
    lowenvaleSprite.setPosition(1050.f, 382.f);
    lowenvaleSprite.setScale(1.35f, 1.35f);
    lowenvaleSprite.setColor(sf::Color(244, 215, 96, 255));

    embercrownTexture.loadFromFile("Textures/embercrown.png");
    embercrownSprite.setTexture(embercrownTexture);
    embercrownSprite.setPosition(298.f, 789.f);
    embercrownSprite.setScale(1.35f, 1.35f);
    embercrownSprite.setColor(sf::Color(189, 126, 74, 255));

    everdellTexture.loadFromFile("Textures/everdell.png");
    everdellSprite.setTexture(everdellTexture);
    everdellSprite.setPosition(295.f, 532.f);
    everdellSprite.setScale(1.35f, 1.35f);
    everdellSprite.setColor(sf::Color(72, 185, 159, 255));

    concordiaTexture.loadFromFile("Textures/concordia.png");
    concordiaSprite.setTexture(concordiaTexture);
    concordiaSprite.setPosition(575.f, 412.f);
    concordiaSprite.setScale(1.35f, 1.35f);
    concordiaSprite.setColor(sf::Color(255, 137, 255, 255));

    updatePositions();
}

void InGame::initUserKingdomName(const std::string &name) {
    this->userKingdomName.setString(name);
}

void InGame::centerView() {
    // Center the sprite in the view
    sf::Vector2u textureSize = mapTexture.getSize();
    mapSprite.setPosition(0.0f, 0.0f);

    // Set view to show the entire map
    gameView.setCenter(static_cast<float>(textureSize.x) / 2.0f,
                      static_cast<float>(textureSize.y) / 2.0f);
}

void InGame::handleTextHover(sf::Text& text, sf::Sprite& associatedSprite, const sf::Event& event) {
    if (!isPressed) {
        // Check if the mouse is over the text
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
            if (text.getGlobalBounds().contains(window->mapPixelToCoords(mousePos))) {
                // Increase the scale of the associated sprite when text is hovered
                float hoverScale = BASE_SCALE * HOVER_SCALE_MULTIPLIER * scaleFactor;
                associatedSprite.setScale(hoverScale, hoverScale);
                // Play sound only when we first hover over the text
                if (!wasHovered) {
                    hoverSound.play();
                }
                wasHovered = true;
                hoveredText = &text;
            } else if (hoveredText == &text) {
                // Reset the scale when not hovered
                float normalScale = BASE_SCALE * scaleFactor;
                associatedSprite.setScale(normalScale, normalScale);
                hoveredText = nullptr;
                wasHovered = false;
            }
        }
    }
}

// Modify the handleTextClick function
void InGame::handleTextClick(sf::Text& text, sf::Sprite& associatedSprite, const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
        if (text.getGlobalBounds().contains(window->mapPixelToCoords(mousePos))) {
            if (firstClick) {
                text.setString(userKingdomName.getString() != "" ? userKingdomName.getString() : text.getString());
                firstClick = false;
                kingdomChosen = true;
                playerKingdom = &text;
                playerKingdomColor = getCurrentColor(&text);  // Store initial kingdom color
            } else {
                // Only initiate battle if the first clicked territory matches player's kingdom color
                if (clickedText != nullptr && clickedText != &text) {
                    sf::Color clickedColor = getCurrentColor(clickedText);
                    // Check if the previous clicked text's color matches player's kingdom color
                    if (clickedColor == playerKingdomColor) {
                        previousClickedText = clickedText;
                        previousClickedColor = getCurrentColor(previousClickedText);
                        currentClickedColor = getCurrentColor(&text);

                        sf::Vector2f currentPos = text.getPosition();
                        sf::Vector2f previousPos = clickedText->getPosition();
                        sf::Vector2f midPoint(
                            (currentPos.x + previousPos.x) / 2.f,
                            (currentPos.y + previousPos.y) / 2.f
                        );
                        startDiceAnimation(midPoint);
                    } else {
                        // Reset selection if player didn't start from their territory
                        clickedText = nullptr;
                        isPressed = false;
                        return;
                    }
                }

                if (clickedText == &text) {
                    associatedSprite.setColor(getCurrentColor(&text));
                    clickedText = nullptr;
                    isPressed = false;
                } else {
                    clickedText = &text;
                    isPressed = true;
                }
            }
        }
    }
}


void InGame::update(const sf::Event& event) {
    handleInput(event);

    if (isPaused) {
        pauseMenu->update(event);
        return;
    }

    if (isGameOver || isVictory) {
        // Handle exit button click
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
            sf::Vector2f worldPos = window->mapPixelToCoords(mousePos, window->getDefaultView());

            if (exitButton.getGlobalBounds().contains(worldPos)) {
                window->close();
            }
        }
        return;
    }

    // Update dice animation
    updateDiceAnimation();

    // Check for game over or victory condition after each battle
    if (!isDiceAnimating && kingdomChosen) {
        isGameOver = checkGameOver();
        if (isGameOver || isVictory) {
            // Center text and exit button
            sf::Vector2u windowSize = window->getSize();

            if (isVictory) {
                victoryText.setPosition(
                    static_cast<float>(windowSize.x) / 2.f - victoryText.getGlobalBounds().width / 2.f,
                    static_cast<float>(windowSize.y) / 2.f - victoryText.getGlobalBounds().height / 2.f - 50.f
                );
            } else {
                gameOverText.setPosition(
                    static_cast<float>(windowSize.x) / 2.f - gameOverText.getGlobalBounds().width / 2.f,
                    static_cast<float>(windowSize.y) / 2.f - gameOverText.getGlobalBounds().height / 2.f - 50.f
                );
            }

            exitButton.setPosition(
                static_cast<float>(windowSize.x) / 2.f - exitButton.getSize().x / 2.f,
                static_cast<float>(windowSize.y) / 2.f + 50.f
            );

            exitButtonText.setPosition(
                static_cast<float>(windowSize.x) / 2.f - exitButtonText.getGlobalBounds().width / 2.f,
                static_cast<float>(windowSize.y) / 2.f + 50.f + (exitButton.getSize().y - exitButtonText.getGlobalBounds().height) / 2.f
            );
        }
    }

    // Handle Hovers and Clicks for text elements
    handleTextHover(elvenIslesTitle, elvenIslesSprite, event);
    handleTextClick(elvenIslesTitle, elvenIslesSprite, event);

    handleTextHover(iceReachTitle, iceReachSprite, event);
    handleTextClick(iceReachTitle, iceReachSprite, event);

    handleTextHover(thrymholdTitle, thrymholdSprite, event);
    handleTextClick(thrymholdTitle, thrymholdSprite, event);

    handleTextHover(solmareTitle, solmareSprite, event);
    handleTextClick(solmareTitle, solmareSprite, event);

    handleTextHover(lowenvaleTitle, lowenvaleSprite, event);
    handleTextClick(lowenvaleTitle, lowenvaleSprite, event);

    handleTextHover(embercrownTitle, embercrownSprite, event);
    handleTextClick(embercrownTitle, embercrownSprite, event);

    handleTextHover(everdellTitle, everdellSprite, event);
    handleTextClick(everdellTitle, everdellSprite, event);

    handleTextHover(concordiaTitle, concordiaSprite, event);
    handleTextClick(concordiaTitle, concordiaSprite, event);
}

void InGame::renderGameOver() {
    // Switch to default view for UI elements
    window->setView(window->getDefaultView());

    // Draw semi-transparent background
    window->draw(gameOverBackground);

    // Draw game over text
    window->draw(gameOverText);

    // Draw exit button
    window->draw(exitButton);
    window->draw(exitButtonText);

    // Restore game view
    window->setView(gameView);
}


void InGame::renderVictory() {
    // Switch to default view for UI elements
    window->setView(window->getDefaultView());

    // Draw semi-transparent background
    window->draw(victoryBackground);

    // Draw victory text
    window->draw(victoryText);

    // Draw exit button
    window->draw(exitButton);
    window->draw(exitButtonText);

    // Restore game view
    window->setView(gameView);
}

// Update render method to use current colors
void InGame::renderSprites() {
    window->draw(mapSprite);

    // Draw each sprite with its current color from the map
    elvenIslesSprite.setColor(currentTerritoryColors.at(&elvenIslesTitle));
    iceReachSprite.setColor(currentTerritoryColors.at(&iceReachTitle));
    thrymholdSprite.setColor(currentTerritoryColors.at(&thrymholdTitle));
    solmareSprite.setColor(currentTerritoryColors.at(&solmareTitle));
    lowenvaleSprite.setColor(currentTerritoryColors.at(&lowenvaleTitle));
    embercrownSprite.setColor(currentTerritoryColors.at(&embercrownTitle));
    everdellSprite.setColor(currentTerritoryColors.at(&everdellTitle));
    concordiaSprite.setColor(currentTerritoryColors.at(&concordiaTitle));

    window->draw(elvenIslesSprite);
    window->draw(iceReachSprite);
    window->draw(thrymholdSprite);
    window->draw(solmareSprite);
    window->draw(lowenvaleSprite);
    window->draw(embercrownSprite);
    window->draw(everdellSprite);
    window->draw(concordiaSprite);
}

void InGame::renderText() const {
    window->draw(this->elvenIslesTitle);
    window->draw(this->iceReachTitle);
    window->draw(this->thrymholdTitle);
    window->draw(this->solmareTitle);
    window->draw(this->lowenvaleTitle);
    window->draw(this->embercrownTitle);
    window->draw(this->everdellTitle);
    window->draw(this->concordiaTitle);
    if (!this->kingdomChosen) {
        window->draw(this->chooseKingdomBackground);
        window->draw(this->chooseKingdomText);
    }
}

void InGame::render() {
    // Set the custom view
    window->setView(gameView);

    // Draw the map and other game elements
    this->renderSprites();
    this->renderText();
    this->renderDiceAnimation();

    // If paused, draw the pause menu with default view
    if (isPaused) {
        window->setView(window->getDefaultView());
        pauseMenu->render();
        window->setView(gameView);  // Restore the game view
    }

    // If game is over, draw game over screen
    if (isGameOver) {
        renderGameOver();
    } else if (isVictory) {
        renderVictory();
    }
}