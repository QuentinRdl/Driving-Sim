#ifndef CIRCUIT_H
#define CIRCUIT_H
#define debug

#include <map>
#include <SFML/Graphics.hpp>
#include "GraphicsUtils/RoadTextureGenerator.h"

class Circuit {
public:

    Circuit(const Game *game);

    const Game *game;

    struct Segment {
        int id;
        RoadTexture* texture;
        float rotation; // Rotation in degrees
        sf::Vector2f realPoint1;
        sf::Vector2f realPoint2;
    };

    std::map<int, Segment> segments;

    static sf::Vector2f rotatePoint(const sf::Vector2f& origin, const sf::Vector2f &point, float angle);

    void set(int id, RoadTexture *texture);
    void set(int id, RoadTexture *texture, sf::Vector2f originPoint);
    void join(int fromId, int toId, RoadTexture *road_texture, float rotation = 0);
    void renderOn(sf::RenderWindow& window) const;

#ifdef debug
    static sf::CircleShape createCircle(sf::Color color, float radius); // TODO DELETE
#endif
};

#endif // CIRCUIT_H
