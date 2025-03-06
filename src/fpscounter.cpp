#include "fpscounter.h"
#include "dejavu_sans_mono_ttf.h"

#include <iostream>

FPSCounter::FPSCounter(): timeAccumulator(0.f), frameCount(0), currentFPS(0), enabled(true) {
    if (!font.loadFromMemory(dejavu_sans_mono_ttf, dejavu_sans_mono_ttf_len)) {
        throw std::runtime_error("Failed to load Arial font from memory.");
    }
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition(10.f, 10.f);
}

void FPSCounter::update(const float dt) {
    if (!enabled) return;
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

void FPSCounter::renderOn(sf::RenderWindow &window) const {
    if (enabled)
        window.draw(text);
}

void FPSCounter::toggle() {
    enabled = !enabled;
}


int FPSCounter::getFPS() const {
    if (!enabled) {
        std::cerr << "FPSCounter::getFPS() triggered while the FPSCounter is disabled." << std::endl;
        return -1;
    }
    return currentFPS;
}
