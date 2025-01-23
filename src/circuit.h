#ifndef CIRCUIT_H
#define CIRCUIT_H

#define SPRITE_SCALE 0.2f

#include "game.h"
#include <map>
#include <SFML/Graphics.hpp>

class Circuit {


    struct Segment {
        int id;
        Game::RoadTexture* texture;
        float rotation; // Rotation in degrees
        sf::Vector2f point1;
        sf::Vector2f point2;
    };

    std::map<int, Segment> segments;

    static sf::Vector2f rotatePoint(const sf::Vector2f& point, float angle);

public:
    void set(int id, Game::RoadTexture* texture, float x, float y);
    void join(int fromId, int toId, Game::RoadTexture* texture, float rotation = 0);
    void render(sf::RenderWindow& window) const;
};

#endif // CIRCUIT_H
