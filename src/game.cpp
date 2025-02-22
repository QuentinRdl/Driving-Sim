#include "game.h"

#include "circuit.h"
#include "GraphicsUtils/roadtexturegenerator.h"


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
    window = std::make_unique<sf::RenderWindow>(videoMode, "Interface Graphique", sf::Style::Titlebar | sf::Style::Close);
    window->setFramerateLimit(30); // todo increase fps.
    texture_manager = {};
}

/**
 * Destructor
 */
Game::~Game() = default;

void Game::manageEvents() {
    while (this->window->pollEvent(this->event)) {
        switch (this->event.type) {
            case sf::Event::KeyPressed:
                if (this->event.key.code == sf::Keyboard::Add) this->zoom_factor += 0.01;
                if (this->event.key.code == sf::Keyboard::Subtract) this->zoom_factor > 0 ? this->zoom_factor -= 0.01 : this->zoom_factor = 0;
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


    circ.setOrigin(SegmentType::Value::LARGE_TURN,
        { static_cast<float>(window->getSize().x) / 3, static_cast<float>(window->getSize().y) / 3 },
                   0.f, true, true);
    circ.join(SegmentType::Value::LARGE_TURN, 270);
    circ.join(SegmentType::Value::LARGE_TURN, 0);
    circ.join(SegmentType::Value::LARGE_TURN, 90);

    circ.renderOn(*this->window);

    this->window->display();
}

bool Game::running() const {
    return this->window->isOpen();
}
