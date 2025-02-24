#ifndef ROADTEXTUREGENERATOR_H
#define ROADTEXTUREGENERATOR_H

#include <complex>

#include "game.h"
#include "ResourceType.h"
#include "texturemanager.h"

struct RoadTexture {
    const sf::Texture *texture = nullptr;
    sf::Sprite sprite;
    sf::Vector2f point1;
    sf::Vector2f point2;
};

/**
 *
 * @param type the type of the segment
 * @param size the size of the texture
 * @param mirror_x true if the texture should be mirrored on the x-axis, false otherwise
 * @param mirror_y true if the texture should be mirrored on the y-axis, false otherwise
 * @return an array of two `sf::Vector2f`, the graphic extremities of the segment
 */
[[nodiscard]] inline const sf::Vector2f *getExtremities(const ResourceType::Value type, const sf::Vector2u& size, const bool mirror_x = false, const bool mirror_y = false) {
    static sf::Vector2f extremities[2];
    switch (type) {
        case ResourceType::SEGMENT_SMALL_STRAIGHT:
            extremities[0] = {0.f, 185.f};
            extremities[1] = {442.f, 185.f};
            break;
        case ResourceType::SEGMENT_LONG_STRAIGHT:
            extremities[0] = {0.f, 188.f};
            extremities[1] = {935.f, 188.f};
            break;
        case ResourceType::SEGMENT_SMALL_TURN:
            extremities[0] = {0.f, 182.f};
            extremities[1] = {345.f, 531.f};
            break;
        case ResourceType::SEGMENT_MEDIUM_TURN:
            extremities[0] = {0.f, 184.f};
            extremities[1] = {712.f, 879.f};
            break;
        case ResourceType::SEGMENT_LARGE_TURN:
            extremities[0] = {0.f, 191.f};
            extremities[1] = {1076.f, 1269.f};
            break;
        case ResourceType::SEGMENT_S_TURN:
            extremities[0] = {0.f, 188.f};
            extremities[1] = {700.f, 915.f};
            break;
        case ResourceType::SEGMENT_U_TURN:
            extremities[0] = {0.f, 218.f};
            extremities[1] = {0.f, 947.f};
            break;
        default: throw std::invalid_argument("Invalid segmentType: not implemented yet maybe ?");
    }

    // Appliquer la transformation de miroir sur l'axe X
    if (mirror_x) {
        extremities[0].x = static_cast<float>(size.x) - extremities[0].x;
        extremities[1].x = static_cast<float>(size.x) - extremities[1].x;
    }
    // Appliquer la transformation de miroir sur l'axe Y
    if (mirror_y) {
        extremities[0].y = static_cast<float>(size.y) - extremities[0].y;
        extremities[1].y = static_cast<float>(size.y) - extremities[1].y;
    }

    return extremities;
}

/**
 * Generate a `RoadTexture` object from a `SegmentType::Value`.
 * Need a `TextureManager` to get the texture.
 *
 * @param texture_manager The texture manager to get a `sf::Texture` from a `SegmentType::Value`
 * @param segment_type The segment type to get the texture from
 * @param mirror_x true if the texture should be mirrored on the x-axis, false otherwise
 * @param mirror_y true if the texture should be mirrored on the y-axis, false otherwise
 * @return a `RoadTexture` object containing the sprite, the texture and its extremities
 */
[[nodiscard]] inline RoadTexture generate_road_texture(const TextureManager &texture_manager, const ResourceType::Value segment_type,
                                         const bool mirror_x = false, const bool mirror_y = false) {
    const sf::Texture& baseTexture = texture_manager.getTexture(segment_type);

    const sf::Vector2f *extremities = getExtremities(segment_type, baseTexture.getSize(), mirror_x, mirror_y);

    RoadTexture road_texture;
    road_texture.point1 = extremities[0];
    road_texture.point2 = extremities[1];
    road_texture.texture = &baseTexture;
    road_texture.sprite = sf::Sprite(*road_texture.texture);

    const int base_x = static_cast<int>(baseTexture.getSize().x);
    const int base_y = static_cast<int>(baseTexture.getSize().y);

    sf::IntRect rect(0, 0, base_x, base_y);
    if (mirror_x) {
        rect.left = base_x;
        rect.width = -base_x;
    }
    if (mirror_y) {
        rect.top = base_y;
        rect.height = -base_y;
    }
    road_texture.sprite.setTextureRect(rect);

    return road_texture;
}

#endif // ROADTEXTUREGENERATOR_H
