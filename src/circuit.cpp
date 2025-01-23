#include "circuit.h"
#include <cmath>

// Helper to rotate a point around the origin
sf::Vector2f Circuit::rotatePoint(const sf::Vector2f& point, const float angle) {
    const float rad = angle * static_cast<float>(M_PI) / 180.0f;
    const float cosA = std::cos(rad);
    const float sinA = std::sin(rad);
    return {
        point.x * cosA - point.y * sinA,
        point.x * sinA + point.y * cosA
    };
}

// Define a segment in the circuit
void Circuit::set(const int id, Game::RoadTexture* texture, const float x, const float y) {
    segments[id] = { id, texture, 0 };

    segments[id].texture->sprite.setScale(SPRITE_SCALE, SPRITE_SCALE);
    segments[id].texture->sprite.setPosition(x, y);

    segments[id].texture->point1 = { 50, 50 }; // Start at origin
    segments[id].texture->point2 = { texture->sprite.getGlobalBounds().width, texture->sprite.getGlobalBounds().height };
}

// Connect two segments
void Circuit::join(const int fromId, const int toId, Game::RoadTexture* texture, const float rotation) {
    if (segments.find(fromId) == segments.end()) {
        throw std::runtime_error("Segment with fromId not found");
    }

    // Get the ending point of the 'from' segment
    const auto& fromSegment = segments[fromId];
    const auto& fromPoint2 = fromSegment.texture->point2;

    // Calculate the offset after rotation
    const auto rotatedOffset = rotatePoint({ 0, texture->sprite.getGlobalBounds().height }, rotation);

    // Set new segment's points
    texture->point1 = fromPoint2;
    texture->point2 = fromPoint2 + rotatedOffset;
    texture->sprite.setScale(SPRITE_SCALE, SPRITE_SCALE);

    segments[toId] = { toId, texture, rotation };
}

// Render all segments
void Circuit::render(sf::RenderWindow& window) const {
    for (const auto& [id, segment] : segments) {
        segment.texture->sprite.setPosition(segment.texture->point1);
        segment.texture->sprite.setRotation(segment.rotation);
        window.draw(segment.texture->sprite);
    }
}
