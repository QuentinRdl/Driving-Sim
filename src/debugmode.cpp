#include "debugmode.h"

#include <cmath>
#include "dejavu_sans_mono_ttf.h"
#include "calculationhelper.h"

DebugMode::DebugMode(): enabled(false) {
    if (!font.loadFromMemory(dejavu_sans_mono_ttf, dejavu_sans_mono_ttf_len)) {
        throw std::runtime_error("Failed to load Arial font from memory.");
    }
}

void DebugMode::toggle() {
    enabled = !enabled;
}

bool DebugMode::isEnabled() const {
    return enabled;
}

void DebugMode::renderOn(sf::RenderWindow &window) const {
    if (!enabled) return;

    const sf::Vector2u windowSize = window.getSize();

    for (size_t i = 0; i < texts.size(); ++i) {
        sf::Text text = texts.at(i);
        sf::RectangleShape bg = backgrounds.at(i);

        constexpr float lineSpaceing = 25.f;
        constexpr float topMargin = 20.f;
        constexpr float margin = 20.f;
        const float textWidth = text.getLocalBounds().width;

        sf::Vector2f pos(static_cast<float>(windowSize.x) - textWidth - margin,
                         topMargin + static_cast<float>(i) * lineSpaceing);

        text.setPosition(pos);
        bg.setPosition(pos * 1.f);

        window.draw(bg);
        window.draw(text);
    }
}

void DebugMode::update(const Car *car) {
    texts.clear();

    auto round = [](const double value, const int precision = 3) {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), ("%." + std::to_string(precision) + "f").c_str(), value);
        return std::string(buffer);
    };
    auto &v = car->vehicle;

    const sf::Color gray(120, 120, 120);

    addLine("Car", 28, sf::Color(255, 84, 71));
    addLine("Constants: ", 25, gray);
    addLine("Mass: " + round(v->mass, 0), 20);
    addLine("Coeff Friction (Front): " + round(v->mu_front, 2), 20);
    addLine("Coeff Friction (Rear): " + round(v->mu_rear, 2), 20);
    addLine("Moment d'inertie: " + round(v->I, 1), 20);
    addLine("Constante gravitationnelle: " + round(v->g, 2), 20);
    addLine("Longitudinal rigidity (N): " + round(v->Cx, 0), 20);
    addLine("Lateral rigidity (N/rad): " + round(v->Cy, 0), 20);
    addLine("", 10);
    addLine("Variables: ", 25, gray);
    addLine("Position (X/Y): " + round(v->x, 1) + " / " + round(v->y, 1), 20);
    addLine("Speed: " + round(v->vx), 20);
    addLine("PSI (in deg): " + round(std::fmod(radToDeg(v->psi), 360.0f)), 20); // direction in ° mod 360
    addLine("Lacet (in deg/s): " + round(radToDeg(v->lacet)), 20); // Lacet in ° mod 360
}

void DebugMode::addLine(const std::string &content, const uint size, const sf::Color &color) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setString(content);

    sf::RectangleShape background;
    background.setSize(sf::Vector2f(text.getLocalBounds().width*1.1f, text.getLocalBounds().height*1.1f));
    background.setFillColor(sf::Color(0, 0, 0, 255/2));

    texts.push_back(text);
    backgrounds.push_back(background);
}
