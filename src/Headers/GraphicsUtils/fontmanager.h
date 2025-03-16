#ifndef FONTMANAGER_H
#define FONTMANAGER_H
#include <memory>
#include <SFML/Graphics.hpp>

#include "dejavu_sans_mono_ttf.h"

class FontManager {

    std::unique_ptr<sf::Font> font;

public:
    FontManager() {
        font = std::make_unique<sf::Font>();
        if (!font->loadFromMemory(dejavu_sans_mono_ttf, dejavu_sans_mono_ttf_len)) {
            throw std::runtime_error("Failed to load font");
        }
    }

    [[nodiscard]] sf::Font& getFont() const { return *font; }
};

#endif //FONTMANAGER_H
