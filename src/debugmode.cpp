#include "debugmode.h"

#include <cmath>
#include "calculationhelper.h"

DebugMode::DebugMode(const FontManager& font_manager): font(font_manager.getFont()), enabled(false) {}

void DebugMode::toggle() {
    enabled = !enabled;
}

bool DebugMode::isEnabled() const {
    return enabled;
}

void DebugMode::renderOn(sf::RenderWindow &window) const {
    if (!enabled) return;

    const sf::Vector2u windowSize = window.getSize();
    constexpr float lineSpaceing = 25.f;
    constexpr float topMargin = 20.f;
    constexpr float margin = 20.f;

    for (int i = 0; i < backgrounds.size(); ++i) {
        sf::RectangleShape bg = backgrounds.at(i);
        sf::Text text = texts.at(i);
        const float textWidth = text.getLocalBounds().width;

        sf::Vector2f pos(static_cast<float>(windowSize.x) - textWidth - margin,
                         topMargin + static_cast<float>(i) * lineSpaceing);

        bg.setPosition(pos + sf::Vector2f(-5.0f, 0.0f));

        window.draw(bg);
    }
    for (size_t i = 0; i < texts.size(); ++i) {
        sf::Text text = texts.at(i);

        const float textWidth = text.getLocalBounds().width;

        sf::Vector2f pos(static_cast<float>(windowSize.x) - textWidth - margin,
                         topMargin + static_cast<float>(i) * lineSpaceing);
        text.setPosition(pos);
        window.draw(text);
    }
}

void DebugMode::update(const Car *car) {
    texts.clear();
    backgrounds.clear();

    auto round = [](const double value, const int precision = 3) {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), ("%." + std::to_string(precision) + "f").c_str(), value);
        return std::string(buffer);
    };
    auto &v = car->vehicle;

    const sf::Color gray(200, 200, 200);

    addLine("Car", 28, sf::Color(255, 84, 71));
    addLine("Constants:", 25, gray);
    addLine("Mass: " + round(v->mass, 0), 20);
    addLine("Coeff Friction (Front): " + round(v->mu_front, 2), 20);
    addLine("Coeff Friction (Rear): " + round(v->mu_rear, 2), 20);
    addLine("Moment d'inertie: " + round(v->I, 1), 20);
    addLine("Longitudinal rigidity (N): " + round(v->Cx, 0), 20);
    addLine("Lateral rigidity (N/rad): " + round(v->Cy, 0), 20);
    addLine("", 10);
    addLine("Variables:", 25, gray);
    addLine("Position (X/Y): " + round(v->x, 1) + " / " + round(v->y, 1), 20);
    addLine("Speed (in km/h): " + round(v->vx*3.6), 20);
    addLine("Heading (in deg): " + round(std::fmod(radToDeg(v->psi), 360.0f)), 20); // heading in ° mod 360
    addLine("Steering Wheel (in deg): " + round(car->getSteeringAngle()), 20);
    addLine("Lacet (in deg/s): " + round(radToDeg(v->lacet)), 20); // Lacet in ° mod 360
}

void DebugMode::addLine(const std::string &content, const unsigned int size, const sf::Color &color) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setString(content);

    sf::RectangleShape background;
    background.setSize(sf::Vector2f(text.getLocalBounds().width*1.3f, text.getLocalBounds().height + 25.f/4.f));
    background.setFillColor(sf::Color(64, 64, 64, 255/2));

    texts.push_back(text);
    backgrounds.push_back(background);
}
