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
    window->setFramerateLimit(200); // todo increase fps.
    texture_manager = {};
}

/**
 * Destructor
 */
Game::~Game() = default;

/**
 * Manage the events
 * Here is the list of events that are managed (KeyPressed : Description):
 * => F : toggle the FPS counter
 * => ESC : close the window
 * Zoom is managed from the mouse wheel.
 */
void Game::manageEvents() {
    while (this->window->pollEvent(this->event)) {
        switch (this->event.type) {
            case sf::Event::KeyPressed:
                if (this->event.key.code == sf::Keyboard::F)            fps_counter.toggle();
                else if (this->event.key.code == sf::Keyboard::Escape)  this->window->close();
                break;
            case sf::Event::MouseWheelScrolled:
                if (this->event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    if (this->event.mouseWheelScroll.delta > 0)
                            this->zoom_factor += 0.01;
                    else    this->zoom_factor = (this->zoom_factor > 0 ? this->zoom_factor - 0.01 : 0);
                }
                break;
            case sf::Event::Closed:
                this->window->close();
                break;
            default: break;
        }
    }

    // Mise à jour du compteur de FPS
    fps_counter.update();
}


void Game::update() {
    this->manageEvents();
}

void Game::render() const {
    this->window->clear();

    Circuit circ(this);

    circ.setOrigin(SegmentType::Value::LARGE_TURN, { 500, 500 }, 0.f, true, true);
    circ.join(SegmentType::Value::LARGE_TURN, 270);
    circ.join(SegmentType::Value::LARGE_TURN, 0);
    circ.join(SegmentType::Value::LARGE_TURN, 90);

    circ.renderOn(*this->window);

    fps_counter.draw(*this->window);

    this->window->display();
}

bool Game::running() const {
    return this->window->isOpen();
}
