//
// Created by rgld_ on 24/02/25.
//

#include "fpscounter.h"
#include "arial_ttf.h"

#include <iostream>

FPSCounter::FPSCounter(): timeAccumulator(0.f), frameCount(0), currentFPS(0), enabled(true) {
    if (!font.loadFromMemory(arial_ttf, arial_ttf_len)) {
        throw std::runtime_error("Failed to load Arial font from memory.");
    }
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition(10.f, 10.f);
}

void FPSCounter::update() {
    if (!enabled) return;
    const float dt = clock.restart().asSeconds();
    timeAccumulator += dt;
    frameCount++;

    if (timeAccumulator >= 1.f) {
        currentFPS = frameCount;
        frameCount = 0;
        timeAccumulator = 0.f;
        text.setString("FPS: " + std::to_string(currentFPS));
        std::cout << "FPS : " << currentFPS << std::endl;
    }
}

void FPSCounter::draw(sf::RenderWindow &window) const {
    if (enabled)
        window.draw(text);
}

void FPSCounter::toggle() {
    enabled = !enabled;
}


int FPSCounter::getFPS() const {
    if (!enabled) {
        std::cerr << "getFPS() triggered while the FPSCounter is disabled." << std::endl;
        return -1;
    }
    return currentFPS;
}



