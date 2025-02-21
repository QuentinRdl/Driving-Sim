#ifndef CIRCUIT_H
#define CIRCUIT_H
#define debug

#include <map>
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

    struct Segment {
        int id;
        RoadTexture texture;
        float rotation{}; // Rotation in degrees
        sf::Vector2f realPoint1;
        sf::Vector2f realPoint2;
    };

    std::map<int, Segment> segments;

    static sf::Vector2f rotatePoint(const sf::Vector2f& origin, const sf::Vector2f &point, float angle);

    void setOrigin(const SegmentType::Value &segment);
    void setOrigin(const SegmentType::Value &segment, sf::Vector2f originPoint);
    void join(const SegmentType::Value &segment, float rotation = 0);
    void renderOn(sf::RenderWindow& window) const;

#ifdef debug
    static sf::CircleShape createCircle(sf::Color color, float radius); // TODO DELETE
#endif
};

#endif // CIRCUIT_H
