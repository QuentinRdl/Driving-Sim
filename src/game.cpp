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
Game::Game(): zoom_factor(0.8) {
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "Interface Graphique", sf::Style::Default);
    window->setVerticalSyncEnabled(true);
    texture_manager = {};

    // Data given by QRadlo in his tests (cf Physics4Real#src/TheLastOneISwear.cpp).
    Vehicle vehicle(1700.0, 1.5, 1.5, 0.5, 150000.0, 40000.0, 580, 450, 0);
    car = std::make_unique<Car>(this, vehicle);

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
                                                                        car->vehicle.x, car->vehicle.y, car->vehicle.r);
            break;
            case sf::Event::MouseWheelScrolled:
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    circuit->needUpdate();
                    if (event.mouseWheelScroll.delta > 0)    zoom_factor += 0.01;
                    else                                     zoom_factor = (zoom_factor > 0 ? zoom_factor - 0.01 : 0);
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
    circuit->setOrigin(ResourceType::Value::SEGMENT_LARGE_TURN, { 500, 500 },
        0.f, true, true);
    circuit->join(ResourceType::Value::SEGMENT_LARGE_TURN, 270);
    circuit->join(ResourceType::Value::SEGMENT_LARGE_TURN, 0);
    circuit->join(ResourceType::Value::SEGMENT_LARGE_TURN, 90);
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
