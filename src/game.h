#ifndef GAME_H
#define GAME_H

#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "car.h"
#include "fpscounter.h"
#include "texturemanager.h"

/**
 * Class that acts as the game engine.
 * Wrapper class.
 */
class Game final {

    sf::VideoMode videoMode;
    sf::Event event{};
    float zoom_factor;

    FPSCounter fps_counter;
    std::unique_ptr<Car> car;
    std::unique_ptr<sf::RenderWindow> window;

    sf::Clock clock;

public:
    TextureManager texture_manager;

    Game();

    ~Game();

    void manageEvents();

    [[nodiscard]] float getZoomFactor() const;

    void update();
    void render() const;

    // Accessors
    [[nodiscard]] bool running() const;

};

#endif //GAME_H
