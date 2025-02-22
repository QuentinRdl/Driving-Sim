#include "circuit.h"
#include <cmath>


// Helper to rotate a point around the origin
sf::Vector2f Circuit::rotatePoint(const sf::Vector2f& origin, const sf::Vector2f &point, const float angle) {
    if (angle == 0) return point;

    const float rad = angle * M_PIf / 180.0f;
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

// Define a segment in the circuit
auto Circuit::setOrigin(const SegmentType::Value &segment_type, const sf::Vector2f originPoint,
                        const float rotation, const bool mirror_x, const bool mirror_y) -> void {
    RoadTexture road_texture = generate_road_texture(game->texture_manager, segment_type, mirror_x, mirror_y);

    const sf::Vector2f & scale = road_texture.sprite.getScale();
    road_texture.sprite.setScale(scale.x * game->getZoomFactor(), scale.y * game->getZoomFactor());
    // printf("Scale of segment %d: %f, %f\n", id, scale.x, scale.y);
    road_texture.sprite.setPosition(originPoint);

    constexpr int id = 1;
    segments[id] = { id, road_texture, rotation };
    segments[id].realPoint1 = originPoint + road_texture.point1 * game->getZoomFactor();
    segments[id].realPoint2 = originPoint + road_texture.point2 * game->getZoomFactor();

    lastId = id;
}

// Connect two segments
void Circuit::join(const SegmentType::Value& segment, const float rotation, const bool mirror_x, const bool mirror_y) {
    RoadTexture road_texture = generate_road_texture(game->texture_manager, segment, mirror_x, mirror_y);

    // Get the ending point and rotation of the 'from' segment
    const auto& fromSegment = segments[lastId];
    const auto& fromPoint2 = fromSegment.realPoint2;

    // Set texture properties
    sf::Sprite* spr = &road_texture.sprite;
    const sf::Vector2f& scale = spr->getScale();
    spr->setScale(scale.x * game->getZoomFactor(), scale.y * game->getZoomFactor());
    spr->setOrigin(road_texture.point1);
    spr->setPosition(fromPoint2);

    sf::Vector2f beforeOffsetPoint = spr->getGlobalBounds().getPosition();
    beforeOffsetPoint += road_texture.point2 * game->getZoomFactor();
    const sf::Vector2f rotatedPoint = rotatePoint(fromPoint2, beforeOffsetPoint, rotation);

    spr->setRotation(rotation); // the rotation part should be after the calc of beforeOffsetPoint or its rotate twice and bad

    lastId += 1;

    // Add to segments
    segments[lastId] = { lastId, road_texture, rotation, fromPoint2, rotatedPoint};
}

void Circuit::renderOn(sf::RenderWindow& window) const {
    for (const auto& [id, segment] : segments) {
        window.draw(segment.texture.sprite);
    }
#ifdef debug
    for (const auto& [id, segment] : segments) {
        constexpr float radius = 10.f;
        sf::CircleShape redCirc = createCircle(sf::Color::Red, radius);
        redCirc.setPosition(segment.realPoint1);

        sf::CircleShape greenCirc = createCircle(sf::Color::Green, radius*2);
        greenCirc.setPosition(segment.realPoint2);

        sf::RectangleShape p1 = createSquare(sf::Color::Red, radius*2);
        p1.setPosition(segment.texture.sprite.getPosition());

        sf::RectangleShape p2 = createSquare(sf::Color::Green, radius*2);
        p2.setPosition(segment.texture.sprite.getPosition() + segment.texture.sprite.getGlobalBounds().getSize());


        window.draw(greenCirc);
        window.draw(redCirc);
        window.draw(p1);
        window.draw(p2);
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

sf::RectangleShape Circuit::createSquare(const sf::Color color, const float length) {
    sf::RectangleShape rectangle({length, length});
    rectangle.setFillColor(color);
    return rectangle;
}
#endif