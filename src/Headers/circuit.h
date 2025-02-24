#ifndef CIRCUIT_H
#define CIRCUIT_H
#include "main_classes.h"

// #define debug
#include <SFML/Graphics.hpp>
#include "roadtexturegenerator.h"
#include "game.h"

/* Declaration to avoid circular includes problem */

class Circuit {
    struct RoadSegment {
        RoadTexture texture;
        float rotation{}; // Rotation in degrees
        sf::Vector2f realPoint1;
        sf::Vector2f realPoint2;
    };

    bool should_be_updated = true;

    const Game* game;

    std::vector<RoadSegment> segments;
public:
    /**
     * @param game a pointer to the main game instance.
     */
    explicit Circuit(const Game *game): game(game) {
        segments = {};
    }

    /** Default Constructor */
    ~Circuit() = default;

    /** Default copy operator */
    Circuit& operator=(const Circuit& other) = default;

    static sf::Vector2f rotatePoint(const sf::Vector2f& origin, const sf::Vector2f &point, float angle);

    void setOrigin(const ResourceType::Value &segment_type, sf::Vector2f originPoint = {0.f, 0.f}, float rotation = 0, bool mirror_x = false, bool mirror_y = false);
    void join(const ResourceType::Value &segment_type, float rotation = 0, bool mirror_x = false, bool mirror_y = false);
    void renderOn(sf::RenderWindow& window) const;

    void resetSegments();

    bool shouldBeUpdated() {
        if (should_be_updated) {
            should_be_updated = false;
            return true;
        }
        return false;
    }
    void needUpdate() { should_be_updated = true; }
#ifdef debug
    /* Debug function for the sprite */
    static sf::CircleShape createCircle(sf::Color color, float radius);
    static sf::RectangleShape createSquare(sf::Color color, float length);
#endif
};

#endif // CIRCUIT_H
