#include "RoadTextureGenerator.h"


RoadTexture generate_road_texture(const SegmentType::Value type) {
    const sf::Vector2f *extremities = getExtremities(type);

    RoadTexture road_texture;
    road_texture.point1 = extremities[0];
    road_texture.point2 = extremities[1];
    road_texture.texture.loadFromFile(SegmentType::getPath(type));
    road_texture.sprite = sf::Sprite(road_texture.texture);

    return road_texture;
}

const sf::Vector2f * getExtremities(const SegmentType::Value type) {
    static sf::Vector2f extremities[2];

    switch (type) {
        case SegmentType::SMALL_STRAIGHT:
            extremities[0] = {0.f, 185.f};
            extremities[1] = {442.f, 185.f};
            break;
        case SegmentType::LONG_STRAIGHT:
            extremities[0] = {0.f, 188.f};
            extremities[1] = {935.f, 188.f};
            break;
        case SegmentType::SMALL_TURN:
            extremities[0] = {0.f, 182.f};
            extremities[1] = {345.f, 531.f};
            break;
        case SegmentType::MEDIUM_TURN:
            extremities[0] = {0.f, 184.f};
            extremities[1] = {712.f, 879.f};
            break;
        case SegmentType::LARGE_TURN:
            extremities[0] = {0.f, 191.f};
            extremities[1] = {1076.f, 1269.f};
            break;
        case SegmentType::S_TURN:
            extremities[0] = {0.f, 188.f};
            extremities[1] = {700.f, 915.f};
            break;
        case SegmentType::U_TURN:
            extremities[0] = {0.f, 218.f};
            extremities[1] = {0.f, 947.f};
            break;
        default: throw std::invalid_argument("Invalid segmentType: not implemented yet maybe ?");
    }
    return extremities;
}
