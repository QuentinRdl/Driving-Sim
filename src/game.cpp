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
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "Simulateur de conduite", sf::Style::Default); // TODO check name.
    window->setVerticalSyncEnabled(true);
    texture_manager = {};

    car = std::make_unique<Car>(this);
    const float win_x = static_cast<float>(window->getSize().x);
    const float win_y = static_cast<float>(window->getSize().y);
    game_view.setSize(win_x, win_y);
    game_view.setCenter(car->getX(), car->getY());

    hud_view.setSize(win_x, win_y);
    hud_view.setCenter(win_x / 2.f, win_y / 2.f);

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
                                                                     car->vehicle->x, car->vehicle->y, car->vehicle->lacet);
                else if (event.key.code == sf::Keyboard::F3) {
                    debug_mode.toggle();
                    printf("Debug mode changed to %s\n", debug_mode.isEnabled() ? "ON" : "OFF");
                }
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
    debug_mode.update(car.get());

    if (circuit->shouldBeUpdated()) {
        updateCircuit();
    }

    car->update(dt);

    game_view.setCenter(car->getX(), car->getY());
    game_view.setRotation(radToDeg(car->getPsi() + M_PI_2f)); // Here we add PI/2 to have the good direction (car moving to the top of the screen).
    window->setView(game_view);
}

void Game::updateCircuit() const {
    circuit->resetSegments();
    circuit->setOrigin(ResourceType::Value::SEGMENT_SMALL_STRAIGHT, { 500, 40 });
    circuit->join(ResourceType::Value::SEGMENT_S_TURN);
    circuit->join(ResourceType::Value::SEGMENT_MEDIUM_TURN);
    circuit->join(ResourceType::Value::SEGMENT_U_TURN, 90);
    circuit->join(ResourceType::Value::SEGMENT_U_TURN, 90, true);
    circuit->join(ResourceType::Value::SEGMENT_LONG_STRAIGHT, 90);
    circuit->join(ResourceType::Value::SEGMENT_U_TURN, 90);
    circuit->join(ResourceType::Value::SEGMENT_LARGE_TURN, 90, true);
    circuit->join(ResourceType::Value::SEGMENT_S_TURN, 180, false, false);
    circuit->join(ResourceType::Value::SEGMENT_U_TURN, 180, false, false);
    circuit->join(ResourceType::Value::SEGMENT_LONG_STRAIGHT, 0, false, false);
}

void Game::render() const {
    window->clear();

    /* --- Game View (Car, Circuit, ...) --- */
    window->setView(game_view);
    circuit->renderOn(*window);
    car->renderOn(*window);

    /* --- HUD View (FPS, Menus, ...) --- */
    window->setView(hud_view);
    fps_counter.renderOn(*window);
    debug_mode.renderOn(*window);

    window->display();
}

bool Game::running() const {
    return window->isOpen();
}