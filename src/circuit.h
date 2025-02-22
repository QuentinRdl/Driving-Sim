#ifndef CIRCUIT_H
#define CIRCUIT_H
// #define debug

#include <SFML/Graphics.hpp>
#include "GraphicsUtils/roadtexturegenerator.h"

class Circuit {

    int lastId;

public:
    explicit Circuit(const Game *game): lastId(0), game(game) {
        segments = {};
    }

    /** Default Constructor */
    ~Circuit() = default;

    /** Default copy operator */
    Circuit& operator=(const Circuit& other) = default;

    const Game* game;

    struct RoadSegment {
        RoadTexture texture;
        float rotation{}; // Rotation in degrees
        sf::Vector2f realPoint1;
        sf::Vector2f realPoint2;
    };

    std::vector<RoadSegment> segments;

    static sf::Vector2f rotatePoint(const sf::Vector2f& origin, const sf::Vector2f &point, float angle);

    void setOrigin(const SegmentType::Value &segment_type, sf::Vector2f originPoint = {0.f, 0.f}, float rotation = 0, bool mirror_x = false, bool mirror_y = false);
    void join(const SegmentType::Value &segment_type, float rotation = 0, bool mirror_x = false, bool mirror_y = false);
    void renderOn(sf::RenderWindow& window) const;

#ifdef debug
    /* Debug function for the sprite */
    static sf::CircleShape createCircle(sf::Color color, float radius);
    static sf::RectangleShape createSquare(sf::Color color, float length);
#endif
};

#endif // CIRCUIT_H
