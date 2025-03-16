#ifndef BUTTON_H
#define BUTTON_H
#include <iomanip>
#include <string>
#include <SFML/Graphics.hpp>

#include "dejavu_sans_mono_ttf.h"
#include "fontmanager.h"

/**
 * Class representing a button in the main menu.
 */
class Button {
    sf::RectangleShape shape;
    sf::Text label;
    sf::Font font;
public:
    /**
     *
     * @param size the size of the button
     * @param position the position of the button
     * @param text the text to display on the button
     * @param font_manager the font manager to use
     */
    Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text, const FontManager& font_manager) {
        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(sf::Color::Blue); // TODO change color ??
        if (!font.loadFromMemory(dejavu_sans_mono_ttf, dejavu_sans_mono_ttf_len)) {
            throw std::runtime_error("Failed to load font");
        }
        label.setFont(font_manager.getFont());
        label.setString(text);
        label.setCharacterSize(20); // TODO change size ??
        label.setFillColor(sf::Color::White);
        label.setPosition(position + size /2.f);

        const sf::FloatRect textRect = label.getLocalBounds();
        label.setOrigin(textRect.left + textRect.width / 2.f,
                        textRect.top + textRect.height / 2.f);
    }

    /**
     * Render the button on the window.
     * @param window the window to render the button on
     */
    void renderOn(sf::RenderWindow& window) const {
        window.draw(shape);
        window.draw(label);
    }

    /**
     *
     * @param window the window to check if the button is clicked
     * @return true if the button is clicked, false otherwise
     */
    bool isClicked(const sf::RenderWindow& window) const {
        printf("isClicked called \n");
        return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
    }
};

#endif //BUTTON_H
