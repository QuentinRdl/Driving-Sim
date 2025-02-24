//
// Created by rgld_ on 24/02/25.
//

#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include <SFML/Graphics.hpp>
#include <string>


class FPSCounter {
    float timeAccumulator;
    int frameCount;
    int currentFPS;
    sf::Font font;
    sf::Text text;

    bool enabled;

public:
    /**
     * Constructor
     */
    FPSCounter();

    /**
     * Update the FPS counter<br>
     * Note: if the FPSCounter is disabled, this function does nothing.
     * @param dt the time elapsed since the last frame (in seconds)
     */
    void update(float dt);

    /**
     * Note: if the FPSCounter is disabled, this function does nothing.
     * @param window the window to draw the FPS counter on
     */
    void draw(sf::RenderWindow &window) const;

    /**
     * Toggle the FPS counter on/off
     */
    void toggle();

    /**
     * Get the current FPS
     * @return the current FPS if enabled, -1 otherwise
     */
    int getFPS() const;
};


#endif //FPSCOUNTER_H
