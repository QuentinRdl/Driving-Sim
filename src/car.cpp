#include "car.h"

#include <cmath>

#include <SFML/Window/Keyboard.hpp>

#include "game.h"
#include "resourcetype.h"
#include "calculationhelper.h"


Car::Car(const Game* game, const Vehicle &vehicle): game(game), vehicle(vehicle), currentDelta(0.f), currentSlip(0.f) {
    sprite.setTexture(game->texture_manager.getTexture(ResourceType::CAR));
    const sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Car::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        currentDelta -= deltaIncrement;
        if (currentDelta < -maxDelta) currentDelta = -maxDelta;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        currentDelta += deltaIncrement;
        if (currentDelta > maxDelta) currentDelta = maxDelta;
    }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
        !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        if (std::abs(currentDelta) < deltaIncrement) currentDelta = 0.f;
        else                                         currentDelta += (currentDelta > 0 ? -deltaIncrement : deltaIncrement);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        currentSlip += slipIncrement;
        if (currentSlip > maxSlip)
            currentSlip = maxSlip;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        currentSlip -= slipIncrement;
        if (currentSlip < -maxSlip)
            currentSlip = -maxSlip;
    }

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
        !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        if (std::abs(currentSlip) < slipIncrement)  currentSlip = 0.f;
        else                                        currentSlip += (currentSlip > 0 ?
                                                        -slipIncrement :
                                                        slipIncrement
                                                    );
    }
}

void Car::update(const float dt) {
    handleInput();

    vehicle.updateBicycle(dt, currentDelta, currentSlip);
    sprite.setPosition(vehicle.x, vehicle.y);
    sprite.setRotation(radToDeg(vehicle.psi));
    sf::Vector2f scale = sprite.getScale();
    scale = scale.x == 0 && scale.y == 0 ? scale : sf::Vector2f(1.f, 1.f);
    const float& zoom = game->getZoomFactor();
    sprite.setScale(scale.x * zoom, scale.y * zoom);
}

void Car::renderOn(sf::RenderWindow &window) const {
    window.draw(sprite);
}
