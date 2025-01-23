#include "game.h"

#include "circuit.h"

/**
 * Private
 */
void Game::initVariables() {
    this->window = nullptr;
    this->straight = new RoadTexture;
    this->small_turn = new RoadTexture;
    this->medium_turn = new RoadTexture;
    this->large_turn = new RoadTexture;
}

void Game::initTextures() const {
    const std::string basePath = "/home/rgld_/01-dev-projects/cpp-projects/Driving-Sim/images/cropped/";

    this->straight->texture.loadFromFile(basePath + "road_straight.png");
    this->straight->sprite.setTexture(this->straight->texture);
    this->straight->point1 = {0, 170};
    this->straight->point2 = {463, 170};

    this->small_turn->texture.loadFromFile(basePath + "small_turn.png");
    this->small_turn->sprite.setTexture(this->small_turn->texture);
    this->small_turn->point1 = {0, 0};
    this->small_turn->point2 = {0, 0};

    this->medium_turn->texture.loadFromFile(basePath + "medium_turn.png");
    this->medium_turn->sprite.setTexture(this->medium_turn->texture);
    this->medium_turn->point1 = {0, 0};
    this->medium_turn->point2 = {0, 0};

    this->large_turn->texture.loadFromFile(basePath + "large_turn.png");
    this->large_turn->sprite.setTexture(this->large_turn->texture);
    this->large_turn->point1 = {0, 0};
    this->large_turn->point2 = {0, 0};
}

/**
 * Private
 */
void Game::initWindow() {
    this->videoMode = sf::VideoMode::getDesktopMode();
    this->window = new sf::RenderWindow(this->videoMode, "Interface Graphique", sf::Style::Titlebar | sf::Style::Close);
}

void Game::renderRoadTexture(RoadTexture& rt) const {
    rt.sprite.setPosition(rt.point1.x, rt.point1.y);
    this->window->draw(rt.sprite);
}

/**
 * Constructor
 */
Game::Game() {
    this->initVariables();
    this->initWindow();
    this->initTextures();
}

/**
 * Destructor
 */
Game::~Game() {
    delete this->window;
    delete this->straight;
    delete this->small_turn;
    delete this->medium_turn;
    delete this->large_turn;
}

void Game::manageEvents() {
    while (this->window->pollEvent(this->event)) {
        switch (this->event.type) {
            case sf::Event::KeyPressed:
                if (this->event.key.code != sf::Keyboard::Escape) break;
            case sf::Event::Closed:
                this->window->close();
                break;
            default:
                break;
        }
    }
}

void Game::update() {
    this->manageEvents();
}

void Game::render() const {
    this->window->clear(sf::Color(43, 45, 48));

    Circuit circ;
    circ.set(1, this->straight, 0, 0);
    circ.join(1, 2, this->small_turn);
    circ.join(2, 3, this->straight, 90);
    circ.join(3, 4, this->straight, 45);

    circ.render(*this->window);

    this->window->display();
}

bool Game::running() const {
    return this->window->isOpen();
}
