#include "game.h"

#include <memory>

#include "circuit.h"
#include "GraphicsUtils/RoadTextureGenerator.h"


/**
 * Let the user use '+' or '-' to zoom or zoom out in the window.
 * @return the zoom factor.
 */
float Game::getZoomFactor() const {
    return this->zoom_factor;
}

/**
 * Constructor
 */
Game::Game(): videoMode(sf::VideoMode::getDesktopMode()), zoom_factor(0.2) {
    this->window = new sf::RenderWindow(this->videoMode, "Interface Graphique", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(30); // todo increase fps.
    this->texture_manager = {};
}

/**
 * Destructor
 */
Game::~Game() {
    delete this->window;
}

void Game::manageEvents() {
    while (this->window->pollEvent(this->event)) {
        switch (this->event.type) {
            case sf::Event::KeyPressed:
                if (this->event.key.code == sf::Keyboard::Add) this->zoom_factor += 0.01;
                if (this->event.key.code == sf::Keyboard::Subtract) this->zoom_factor -= 0.01;
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
    this->window->clear();

    Circuit circ(this);

    circ.setOrigin(SegmentType::Value::LONG_STRAIGHT, {500.f, 10.f});
    circ.join(SegmentType::Value::LARGE_TURN, 0);
    circ.join(SegmentType::Value::SMALL_STRAIGHT, 90);
    circ.join(SegmentType::Value::SMALL_TURN, 90);
    // circ.join(SegmentType::Value::LONG_STRAIGHT, 180);
    // circ.join(SegmentType::Value::LARGE_TURN, 180);
    // circ.join(SegmentType::Value::SMALL_STRAIGHT, 270);
    // circ.join(SegmentType::Value::SMALL_TURN, 270);

    circ.renderOn(*this->window);

    this->window->display();
}

bool Game::running() const {
    return this->window->isOpen();
}
