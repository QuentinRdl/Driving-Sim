#include "game.h"

#include <iostream>


/**
 * Let the user use '+' or '-' to zoom or zoom out in the window.
 * @return the zoom factor.
 */
float Game::getZoomFactor() const {
    return zoom_factor;
}

/**
 * Constructor
 */
Game::Game(): zoom_factor(0.4) {
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "Interface Graphique", sf::Style::Default);
    window->setVerticalSyncEnabled(true);
    texture_manager = {};

    // Data given by QRadlo in his tests (cf Physics4Real#src/TheLastOneISwear.cpp).
    car = std::make_unique<Car>(this);

    circuit = std::make_unique<Circuit>(this);
    updateCircuit();
}

/**
 * Destructor
 */
Game::~Game() = default;

/**
 * Manage the events<br>
 * Here is the list of events that are managed (KeyPressed : Description):<br>
 * => F : toggle the FPS counter<br>
 * => ESC : close the window<br>
 * Zoom is managed from the mouse wheel.
 */
void Game::manageEvents() {
    while (window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::F)            fps_counter.toggle();
                else if (event.key.code == sf::Keyboard::Escape)  window->close();
                else if (event.key.code == sf::Keyboard::F7)      printf("Car position: %f / %f\n Lacet: %f\n\n",
                                                                        car->vehicle->x, car->vehicle->y, car->vehicle->r);
            break;
            case sf::Event::MouseWheelScrolled:
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    circuit->needUpdate();
                    if (event.mouseWheelScroll.delta > 0)    zoom_factor += 0.01f;
                    else                                     zoom_factor -= (zoom_factor > 0 ? 0.01f : zoom_factor);
                }
                break;
            case sf::Event::Closed:
                window->close();
                break;
            default: break;
        }
    }
}


void Game::update() {
    const float dt = clock.restart().asSeconds();

    manageEvents();
    fps_counter.update(dt);

    if (circuit->shouldBeUpdated()) {
        updateCircuit();
    }

    car->update(dt);
}

void Game::updateCircuit() const {
    circuit->resetSegments();
    circuit->setOrigin(ResourceType::Value::SEGMENT_SMALL_STRAIGHT, { 500, 40 });
    circuit->join(ResourceType::Value::SEGMENT_S_TURN);
    circuit->join(ResourceType::Value::SEGMENT_MEDIUM_TURN);
    // std::cout << "" << std::endl;
}

void Game::render() const {
    window->clear();

    fps_counter.renderOn(*window);
    circuit->renderOn(*window);
    car->renderOn(*window);

    window->display();
}

bool Game::running() const {
    return window->isOpen();
}
