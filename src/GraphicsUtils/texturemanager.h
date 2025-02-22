#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "segmenttype.h"


/**
 * This class is used to store textures once they are loaded,
 * so that when a resource is needed again, a reference to the already loaded texture can be retrieved.
 */
class TextureManager {
    /**
     * A mutable unordered_map to store the loaded textures.
     * Declared mutable to allow modification from getTexture function as it's a const one.
     */
    mutable std::unordered_map<SegmentType::Value, sf::Texture> textures;

public:
    /**
     * Retrieves a reference to the texture associated with the given segment type/
     * If the texture is not already loaded, it loads it from the corresponding file.
     *
     * @param type The segment type for which to retrieve the texture.
     * @return A const reference to the requested texture.
     * @throws std::runtime_error if the texture cannot be loaded from th efile.
     */
    const sf::Texture& getTexture(const SegmentType::Value type) const {
        if (const auto it = textures.find(type); it != textures.end()) {
            return it->second;
        }
        sf::Texture text;
        if (const std::string path = SegmentType::getPath(type); !text.loadFromFile(path)) {
            throw std::runtime_error("Error while getting the texture in " + path);
        }
        const auto [insert_iterator, success] = textures.emplace(type, std::move(text));
        return insert_iterator->second;
    }
};
#endif //TEXTUREMANAGER_H
