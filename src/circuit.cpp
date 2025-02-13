#include "circuit.h"
#include <cmath>

Circuit::Circuit(const Game * game) {
    this->segments = {};
    this->game = game;
}

// Helper to rotate a point around the origin
sf::Vector2f Circuit::rotatePoint(const sf::Vector2f& origin, const sf::Vector2f &point, const float angle) {
    if (angle == 0) return point;

    const float rad = angle * M_PIf / 180.0;
    const float cosA = std::cos(rad);
    const float sinA = std::sin(rad);

    const float translatedX = point.x - origin.x;
    const float translatedY = point.y - origin.y;

    const float rotatedX = translatedX * cosA - translatedY * sinA;
    const float rotatedY = translatedX * sinA + translatedY * cosA;

    return {
        rotatedX + origin.x,
        rotatedY + origin.y
    };
}

void Circuit::set(const int id, RoadTexture* texture) {
    set(id, texture, {0.f, 0.f});
}

// Define a segment in the circuit
void Circuit::set(const int id, RoadTexture* texture, const sf::Vector2f originPoint) {

    segments[id] = { id, texture, 0 };
    texture->sprite.setScale(game->getZoomFactor(), game->getZoomFactor());
    texture->sprite.setPosition(originPoint);
    segments[id].realPoint1 = originPoint + texture->point1 * game->getZoomFactor();
    segments[id].realPoint2 = originPoint + texture->point2 * game->getZoomFactor();
}

// Connect two segments
void Circuit::join(const int fromId, const int toId, RoadTexture* road_texture, const float rotation) {
    if (segments.find(fromId) == segments.end()) {
        throw std::runtime_error("Segment with fromId not found");
    }

    // Get the ending point and rotation of the 'from' segment
    const auto& fromSegment = segments[fromId];
    const auto& fromPoint2 = fromSegment.realPoint2;

    // Set texture properties
    road_texture->sprite.setScale(game->getZoomFactor(), game->getZoomFactor());
    road_texture->sprite.setOrigin(road_texture->point1);
    road_texture->sprite.setPosition(fromPoint2);

    sf::Vector2f beforeOffsetPoint = road_texture->sprite.getGlobalBounds().getPosition();
    beforeOffsetPoint += road_texture->point2 * game->getZoomFactor();
    const sf::Vector2f rotatedPoint = rotatePoint(fromPoint2, beforeOffsetPoint, rotation);

    road_texture->sprite.setRotation(rotation); // the rotation part should be after the calc of beforeOffsetPoint or its rotate twice and bad

    // Add to segments
    segments[toId] = { toId, road_texture, rotation, fromPoint2, rotatedPoint};
}

void Circuit::renderOn(sf::RenderWindow& window) const {
    for (const auto& [id, segment] : segments) {
        window.draw(segment.texture->sprite);
    }
#ifdef debug
    for (const auto& [id, segment] : segments) {
        constexpr float radius = 10.f;
        sf::CircleShape redCirc = createCircle(sf::Color::Red, radius);
        redCirc.setPosition(segment.realPoint1);

        sf::CircleShape greenCirc = createCircle(sf::Color::Green, radius*2);
        greenCirc.setPosition(segment.realPoint2);

        window.draw(greenCirc);
        window.draw(redCirc);
    }
#endif
}

#ifdef debug
sf::CircleShape Circuit::createCircle(const sf::Color color, const float radius) {
    sf::CircleShape circle(radius);
    circle.setOrigin(radius, radius);
    circle.setFillColor(color);
    return circle;
}
#endif