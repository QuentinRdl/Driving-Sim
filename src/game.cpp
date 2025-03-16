#include "game.h"

#include "button.h"
#include "constants.h"


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
Game::Game(): software_state(this),
              zoom_factor(0.4),
              fps_counter(font_manager), debug_mode(font_manager),
              play_button({0, 0}, {0, 0}, "", font_manager),
              resume_button({0, 0}, {0, 0}, "", font_manager)
{
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "Simulateur de conduite",
                                                sf::Style::Default); // TODO check name.
    // window->setPosition(sf::Vector2i(0, 1080)); // Ecran 1
    texture_manager = {};

    car = std::make_unique<Car>(this);
    const float win_x = static_cast<float>(window->getSize().x);
    const float win_y = static_cast<float>(window->getSize().y);
    game_view.setSize(win_x, win_y);
    game_view.setCenter(car->getX() * METER_TO_PIXEL, car->getY() * METER_TO_PIXEL);

    hud_view.setSize(win_x, win_y);
    hud_view.setCenter(win_x / 2.f, win_y / 2.f);

    play_button = Button({200, 50}, {
                             static_cast<float>(window->getSize().x) / 2.f - 100,
                             static_cast<float>(window->getSize().y) / 2.f - 25
                         }, "Play", font_manager);
    resume_button = Button({200, 50}, {
                               static_cast<float>(window->getSize().x) / 2.f - 100,
                               static_cast<float>(window->getSize().y) / 3.f - 25
                           }, "Resume", font_manager);
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
                switch (software_state()) {
                    case SoftwareState::MAIN_MENU:
                        if (event.key.code == sf::Keyboard::Enter)        software_state.switchTo(SoftwareState::PLAYING);
                        break;
                    case SoftwareState::PLAYING:
                        if (event.key.code == sf::Keyboard::F)            fps_counter.toggle();
                        else if (event.key.code == sf::Keyboard::Escape)  software_state.switchTo(SoftwareState::PAUSED);
                        else if (event.key.code == sf::Keyboard::F3) {
                            debug_mode.toggle();
                            printf("Debug mode changed to %s\n", debug_mode.isEnabled() ? "ON" : "OFF");
                        }
                        else if (event.key.code == sf::Keyboard::F5) {
                            car.reset();
                            car = std::make_unique<Car>(this);
                            game_view.setCenter(car->getX() * METER_TO_PIXEL, car->getY() * METER_TO_PIXEL);
                        }
                    break;
                    case SoftwareState::PAUSED:
                        if (event.key.code == sf::Keyboard::Escape)    software_state.switchTo(SoftwareState::PLAYING);
                    default: break;
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (resume_button.isClicked(*window) || play_button.isClicked(*window)) {
                    software_state.switchTo(SoftwareState::PLAYING);
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
    manageEvents();
    const float dt = clock.restart().asSeconds(); // time delta between the frame N-1 and the frame N
    fps_counter.update(dt);

    switch (software_state()) {
        case SoftwareState::PLAYING:
            debug_mode.update(car.get());

            if (circuit->shouldBeUpdated()) {
                updateCircuit();
            }

            car->update(dt);
            if (car->atTheEdgeOfScreen(game_view)) {
                car->recenterViewOnCar(game_view, dt);
            }
            window->setView(game_view);
            break;
        default: break;
    }
}

void Game::updateCircuit() const {
    circuit->resetSegments();
    circuit->setOrigin(ResourceType::Value::SEGMENT_SMALL_STRAIGHT, { 500, 40 });
    circuit->join(ResourceType::Value::SEGMENT_S_TURN);
    circuit->join(ResourceType::Value::SEGMENT_MEDIUM_TURN);
}

void Game::renderMainMenu() const {
    play_button.renderOn(*window);
}

void Game::render() const {
    window->clear();

    switch (software_state()) {
        case SoftwareState::MAIN_MENU:
            window->setView(window->getDefaultView());
            renderMainMenu();
            break;
        case SoftwareState::PAUSED:
        case SoftwareState::PLAYING:
            /* --- Game View (Car, Circuit, ...) --- */
            window->setView(game_view);
            circuit->renderOn(*window);
            car->renderOn(*window);

            /* --- HUD View (FPS, Menus, ...) --- */
            window->setView(hud_view);
            fps_counter.renderOn(*window);
            debug_mode.renderOn(*window);
            if (software_state() == SoftwareState::PAUSED) {
                sf::RectangleShape overlay(sf::Vector2f(window->getSize()));
                overlay.setFillColor(sf::Color(0, 0, 0, 150));
                window->setView(window->getDefaultView());
                window->draw(overlay);
                resume_button.renderOn(*window);
            }
            break;
        default: break;
    }

    window->display();
}

bool Game::running() const {
    return window->isOpen();
}