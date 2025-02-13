#ifndef GAME_H
#define GAME_H


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

/**
 * Class that acts as the game engine.
 * Wrapper class.
 */
class Game {

    sf::VideoMode videoMode;
    sf::Event event{};
    float zoom_factor;


    void initVariables();
    void initWindow();


public:
    sf::RenderWindow* window{}; // TODO move to private part.

    Game();
    virtual ~Game();

    void manageEvents();

    float getZoomFactor() const;

    void update();
    void render() const;

    // Accessors
    bool running() const;

};

#endif //GAME_H
