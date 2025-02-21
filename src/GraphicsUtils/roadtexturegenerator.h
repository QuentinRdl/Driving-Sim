#ifndef ROADTEXTUREGENERATOR_H
#define ROADTEXTUREGENERATOR_H

#include <iostream>

#include "game.h"
#include "segmenttype.h"
#include "texturemanager.h"

struct RoadTexture {
    sf::Sprite sprite;
    const sf::Texture *texture = nullptr;
    sf::Vector2f point1;
    sf::Vector2f point2;
};

inline const sf::Vector2f *getExtremities(const SegmentType::Value type, const bool mirrorX = false, const bool mirrorY = false) {
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

inline RoadTexture generate_road_texture(const TextureManager &texture_manager, const SegmentType::Value type, const bool mirror_x = false, const bool mirror_y = false) {
    const sf::Vector2f *extremities = getExtremities(type, mirror_x, mirror_y);

    RoadTexture road_texture;
    road_texture.point1 = extremities[0];
    road_texture.point2 = extremities[1];
    road_texture.texture = &texture_manager.getTexture(type);
    road_texture.sprite = sf::Sprite(*road_texture.texture);

    return road_texture;
}

#endif // ROADTEXTUREGENERATOR_H
