#ifndef ROADTEXTUREGENERATOR_H
#define ROADTEXTUREGENERATOR_H

#include "game.h"
#include "segmenttype.h"

struct RoadTexture {
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f point1;
    sf::Vector2f point2;
};

RoadTexture generate_road_texture(SegmentType::Value type);

const sf::Vector2f* getExtremities(SegmentType::Value type);


#endif // ROADTEXTUREGENERATOR_H
