#ifndef GAME_H
#define GAME_H

#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "car.h"
#include "circuit.h"
#include "fpscounter.h"
#include "texturemanager.h"
#include "debugmode.h"

/**
 * Class that acts as the game engine.
 * Wrapper class.
 */
class Game final {

    sf::Event event{};
    float zoom_factor;

    FPSCounter fps_counter;
    DebugMode debug_mode;
    std::unique_ptr<Circuit> circuit;
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
    void updateCircuit() const;

    void render() const;

    // Accessors
    [[nodiscard]] bool running() const;

};

#endif //GAME_H
