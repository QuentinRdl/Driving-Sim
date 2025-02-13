#include "game.h"
#include "circuit.h"
#include "GraphicsUtils/RoadTextureGenerator.h"

/**
 * Private
 */
void Game::initVariables() {
    this->window = nullptr;
    this->zoom_factor = 0.2;
}


/**
 * Private
 */
void Game::initWindow() {
    this->videoMode = sf::VideoMode::getDesktopMode();
    this->window = new sf::RenderWindow(this->videoMode, "Interface Graphique", sf::Style::Titlebar | sf::Style::Close);
}

/**
 * Let the user use '+' or '-' to zoom or zoom out in the wind.
 * @return the zoom factor.
 */
float Game::getZoomFactor() const {
    return this->zoom_factor;
}

/**
 * Constructor
 */
Game::Game() {
    this->initVariables();
    this->initWindow();
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

    RoadTexture rt_straight1 = generate_road_texture(SegmentType::Value::LONG_STRAIGHT);
    RoadTexture rt_turn1 = generate_road_texture(SegmentType::Value::LARGE_TURN);
    RoadTexture rt_straight2 = generate_road_texture(SegmentType::Value::SMALL_STRAIGHT);
    RoadTexture rt_turn2 = generate_road_texture(SegmentType::Value::SMALL_TURN);
    RoadTexture rt_straight3 = generate_road_texture(SegmentType::Value::LONG_STRAIGHT);
    RoadTexture rt_turn3 = generate_road_texture(SegmentType::Value::LARGE_TURN);
    RoadTexture rt_straight4 = generate_road_texture(SegmentType::Value::SMALL_STRAIGHT);
    RoadTexture rt_turn4 = generate_road_texture(SegmentType::Value::SMALL_TURN);

    circ.set(1, &rt_straight1, {500.f, 10.f});
    circ.join(1, 2, &rt_turn1, 0);
    circ.join(2, 3, &rt_straight2, 90);
    circ.join(3, 4, &rt_turn2, 90);
    circ.join(4, 5, &rt_straight3, 180);
    circ.join(5, 6, &rt_turn3, 180);
    circ.join(6, 7, &rt_straight4, 270);
    circ.join(7, 8, &rt_turn4, 270);

    circ.renderOn(*this->window);

    this->window->display();
}

bool Game::running() const {
    return this->window->isOpen();
}
