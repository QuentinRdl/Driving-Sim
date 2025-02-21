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
void Circuit::setOrigin(const SegmentType::Value& segment, const sf::Vector2f originPoint = {0.f, 0.f}, const float rotation = 0) {
    RoadTexture road_texture = generate_road_texture(game->texture_manager, segment);

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
void Circuit::join(const SegmentType::Value& segment, const float rotation) {
    RoadTexture road_texture = generate_road_texture(game->texture_manager, segment);

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
        // printf("Path of segment %d: %s\n", id, segment.texture.sprite.);
        window.draw(segment.texture.sprite);
        std::cout
        << "id: " << segment.id << " / rotation: " << segment.rotation
        << " / RP1: " << segment.realPoint1.x << ";" << segment.realPoint1.y
        << " / RP2: " << segment.realPoint2.x << ";" << segment.realPoint2.y
        << " / Texture size: " << segment.texture.texture->getSize().x << ";" << segment.texture.texture->getSize().y
            << std::endl;
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