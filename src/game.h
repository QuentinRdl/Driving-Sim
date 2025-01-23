#ifndef GAME_H
#define GAME_H


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

/**
 * Class that acts as the game engine.
 * Wrapper class.
 */
class Game {



    sf::RenderWindow* window{};
    sf::VideoMode videoMode;
    sf::Event event{};


    void initVariables();
    void initTextures() const;
    void initWindow();


public:

    Game();
    virtual ~Game();

    void manageEvents();

    void update();
    void render() const;

    // Accessors
    bool running() const;

    struct RoadTexture {
        sf::Sprite sprite;
        sf::Texture texture;
        sf::Vector2f point1;
        sf::Vector2f point2;
    };

    RoadTexture *straight;
    RoadTexture *small_turn;
    RoadTexture *medium_turn;
    RoadTexture *large_turn;
    void renderRoadTexture(RoadTexture &rt) const;

};

#endif //GAME_H
