#ifndef GAME_H
#define GAME_H

#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "button.h"
#include "car.h"
#include "circuit.h"
#include "fpscounter.h"
#include "texturemanager.h"
#include "calculationhelper.h"
#include "debugmode.h"
#include "fontmanager.h"
#include "SoftwareState.h"

/**
 * Class that acts as the game engine.
 * Wrapper class.
 */
class Game final {

    sf::Event event{};
    SoftwareState software_state;
    FontManager font_manager;
    float zoom_factor;

    FPSCounter fps_counter;
    DebugMode debug_mode;

    /* Main Menu Part */
    std::vector<sf::Text> main_menu_texts;
    Button play_button;

    /* Pause Menu Part */
    std::vector<sf::Text> pause_menu_texts;
    Button resume_button;

    /* Play Part */
    std::unique_ptr<Circuit> circuit;
    std::unique_ptr<Car> car;
    std::unique_ptr<sf::RenderWindow> window;

    sf::View game_view;
    sf::View hud_view;

    sf::Clock clock;

public:
    TextureManager texture_manager;

    Game();
    ~Game();

    void manageEvents();

    DebugMode getDebugMode() const { return debug_mode; }

    [[nodiscard]] float getZoomFactor() const;

    void update();
    void render() const;

    // Accessors
    [[nodiscard]] bool running() const;

private:

    void updateCircuit() const;
    void renderMainMenu() const;

    void prepare_main_menu();
    void prepare_pause_menu();
};

#endif //GAME_H
