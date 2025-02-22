#include "circuit.h"
#include <cmath>


/**
 * Rotate a point around an origin by a given angle.
 * @param origin The origin point.
 * @param point The point to rotate.
 * @param angle The angle in degrees.
 * @return The rotated point.
 */
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

/**
 * @param segment_type the type of the segment
 * @param originPoint the origin point of the segment
 * @param rotation the rotation of the segment
 * @param mirror_x true if the segment should be mirrored on the x-axis, false otherwise
 * @param mirror_y true if the segment should be mirrored on the y-axis, false otherwise
 */
void Circuit::setOrigin(const SegmentType::Value &segment_type, const sf::Vector2f originPoint,
                        const float rotation, const bool mirror_x, const bool mirror_y) {
    RoadTexture road_texture = generate_road_texture(game->texture_manager, segment_type, mirror_x, mirror_y);

    const sf::Vector2f & scale = road_texture.sprite.getScale();
    road_texture.sprite.setScale(scale.x * game->getZoomFactor(), scale.y * game->getZoomFactor());
    road_texture.sprite.setPosition(originPoint);

    RoadSegment rs = {road_texture, rotation};
    rs.realPoint1 = originPoint + road_texture.point1 * game->getZoomFactor();
    rs.realPoint2 = originPoint + road_texture.point2 * game->getZoomFactor();

    segments.push_back(rs);
}

/**
 * @param segment_type the type of segment
 * @param rotation the rotation of the segment
 * @param mirror_x true if the segment should be mirrored on the x-axis, false otherwise
 * @param mirror_y true if the segment should be mirrored on the y-axis, false otherwise
 */
void Circuit::join(const SegmentType::Value& segment_type, const float rotation, const bool mirror_x, const bool mirror_y) {
    RoadTexture road_texture = generate_road_texture(game->texture_manager, segment_type, mirror_x, mirror_y);

    // get the ending point and rotation of the 'from' segment
    const auto& fromSegment = segments.back();
    const auto& fromPoint2 = fromSegment.realPoint2;

    sf::Sprite* spr = &road_texture.sprite;
    const sf::Vector2f& scale = spr->getScale();
    spr->setScale(scale.x * game->getZoomFactor(), scale.y * game->getZoomFactor());
    spr->setOrigin(road_texture.point1);
    spr->setPosition(fromPoint2);

    sf::Vector2f beforeOffsetPoint = spr->getGlobalBounds().getPosition();
    beforeOffsetPoint += road_texture.point2 * game->getZoomFactor();
    const sf::Vector2f rotatedPoint = rotatePoint(fromPoint2, beforeOffsetPoint, rotation);

    spr->setRotation(rotation); // the rotation part should be after the calc of beforeOffsetPoint or its rotate twice and bad

    segments.push_back({ road_texture, rotation, fromPoint2, rotatedPoint});
}

void Circuit::renderOn(sf::RenderWindow& window) const {
    for (const auto& seg : segments) {
        window.draw(seg.texture.sprite);
    }
#ifdef debug
    for (const auto& segment : segments) {
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