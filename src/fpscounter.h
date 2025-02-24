//
// Created by rgld_ on 24/02/25.
//

#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include <SFML/Graphics.hpp>
#include <string>


class FPSCounter {
    sf::Clock clock;
    float timeAccumulator;
    int frameCount;
    int currentFPS;
    sf::Font font;
    sf::Text text;

    bool enabled;

public:
    FPSCounter();

    void update();

    void draw(sf::RenderWindow &window) const;

    void toggle();

    int getFPS() const;
};


#endif //FPSCOUNTER_H
