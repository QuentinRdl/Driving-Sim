#ifndef DEBUGMODE_H
#define DEBUGMODE_H

#include <SFML/Graphics.hpp>
#include "car.h"
#include "fontmanager.h"

class DebugMode {

    sf::Font& font;
    std::vector<sf::Text> texts;
    std::vector<sf::RectangleShape> backgrounds;
    bool enabled;

    /**
     * Set up the font, the char size, the color and the position of a `sf::Text`.
     * @param content
     * @param size
     * @param color
     */
    void addLine(const std::string &content, uint size = 20, const sf::Color &color = sf::Color::White);

public:
    /**
     * Constructor
     */
    explicit DebugMode(const FontManager& font_manager);

    /**
     * Toggle the DebugMode on/off
     */
    void toggle();

    /**
     * Check if the DebugMode is enabled
     * @return true if the DebugMode is enabled, false otherwise
     */
    [[nodiscard]] bool isEnabled() const;

    /**
     * Note: if the DebugMode is disabled, this function does nothing.
     * @param window the window to draw the DebugMode on
     */
    void renderOn(sf::RenderWindow &window) const;

    void update(const Car *car);

};

#endif //DEBUGMODE_H
