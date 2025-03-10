#include "car.h"

#include <SFML/Window/Keyboard.hpp>

#include "game.h"
#include "resourcetype.h"
#include "calculationhelper.h"


/**
 * TODO fix the speed increase alone some times ???
 */
Car::Car(const Game* game): game(game), currentDelta(0.0f) {
    constexpr float initSlip = 0;
    constexpr float initSlip_tau = 0.5;
    constexpr float initS_desired = 0.1;
    Vehicle vehicle(1700.0, 1.5, 1.5, 20, 150000.0, 40000.0, initSlip, initSlip_tau, initS_desired, 0.9, 0.9, 9.81);
    this->vehicle = std::make_unique<Vehicle>(vehicle);

    sprite.setTexture(game->texture_manager.getTexture(ResourceType::CAR));
    const sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    carSprite.setTexture(game->texture_manager.getTexture(ResourceType::CAR));
    const sf::FloatRect bounds = carSprite.getLocalBounds();
    carSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

/**
 * TODO Biais lié aux fps => prends pas en compte le dt.
 */
void Car::handleInput() {
    // Incrément pour l'accélération (commande de slip désiré)
    constexpr float throttleIncrement = 0.01f;

    // --- Accélération / Décélération ---
    // Si la flèche du haut est pressée, on augmente la commande d'accélération.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        vehicle->s_desired += throttleIncrement;
        if (vehicle->s_desired > 1.0f)
            vehicle->s_desired = 1.0f; // On limite à 1.0 (valeur maximale)
    }
    // Si la flèche du bas est pressée, on diminue la commande d'accélération (pour freiner).
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        vehicle->s_desired -= throttleIncrement;
        if (vehicle->s_desired < 0.0f)
            vehicle->s_desired = 0.0f; // On ne descend pas en dessous de 0
    }

    // --- Gestion de l'orientation (volant) ---
    // Si la flèche gauche est pressée, on tourne à gauche (angle négatif)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        currentDelta -= deltaIncrement;
        if (currentDelta < -maxDelta)
            currentDelta = -maxDelta;
    }
    // Si la flèche droite est pressée, on tourne à droite (angle positif)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        currentDelta += deltaIncrement;
        if (currentDelta > maxDelta)
            currentDelta = maxDelta;
    }

    // Si aucune touche de direction n'est pressée, on recentre le volant progressivement
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        if (currentDelta > 0) {
            currentDelta -= deltaIncrement;
            if (currentDelta < 0)
                currentDelta = 0;
        } else if (currentDelta < 0) {
            currentDelta += deltaIncrement;
            if (currentDelta > 0)
                currentDelta = 0;
        }
    }
}

void Car::update(const float dt) {
    handleInput();

    // vehicle->updateBicycleRK4(dt, currentDelta);

    vehicleData vd = {};
    vd.delta = currentDelta;

    vehicle->getNextIterations(0, 1, &vd, dt);
    vehicle->setData(vd);
    carSprite.setPosition(vehicle->x, vehicle->y);
    carSprite.setRotation(radToDeg(vehicle->psi));
}


void Car::renderOn(sf::RenderWindow &window) const {
    window.draw(carSprite);
}
