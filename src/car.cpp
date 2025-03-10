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
    Vehicle vehicle(
        1700, // mass
        1.5, 1.5, // distance between the center of gravity and the front and rear axles
        20, // airResistance coefficient
        150000.0f, // Cx
        40000.0f, // Cy
        0.0f, 0.0f, // vx, vy
        0.0f, // lacet
        0.0f, 0.0f, // x, y
        0.0f, // psi (initHeading)
        initSlip, initSlip_tau, initS_desired, // slip, slip_tau, s_desired
        0.9f, 0.9f, // mu_front, mu_rear: coefficients of friction
        9.81f // g: gravity
        );
    this->vehicle = std::make_unique<Vehicle>(vehicle);

    carSprite.setTexture(game->texture_manager.getTexture(ResourceType::CAR));
    const sf::FloatRect bounds = carSprite.getLocalBounds();
    carSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);


    carPrediction = sf::VertexArray(sf::LineStrip, pointsCount);

}

/**
 * TODO Biais lié aux fps => prends pas en compte le dt.
 */
void Car::handleInput(const float dt) {

    constexpr float throttleRate = 0.1f;        // by second
    constexpr float steeringRate   = 0.1f;     // by second

    // Incrément pour l'accélération (commande de slip désiré)
    const float throttleIncrement = throttleRate * dt;
    const float deltaIncrementScaled = steeringRate * dt;

    // --- Accélération / Décélération ---
    // Si la flèche du haut est pressée, on augmente la commande d'accélération.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        vehicle->s_desired = std::min(vehicle->s_desired + throttleIncrement, 1.0f);
    }
    // Si la flèche du bas est pressée, on diminue la commande d'accélération (pour freiner).
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        vehicle->s_desired = std::max(vehicle->s_desired - throttleIncrement, 0.0f);
    }


    constexpr float maxDelta = 0.3f;         // Max angle for the steering wheel

    // --- Gestion de l'orientation (volant) ---
    // Si la flèche gauche est pressée, on tourne à gauche (angle négatif)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        currentDelta -= deltaIncrementScaled;
        if (currentDelta < -maxDelta)
            currentDelta = -maxDelta;
    }
    // Si la flèche droite est pressée, on tourne à droite (angle positif)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        currentDelta += deltaIncrementScaled;
        if (currentDelta > maxDelta)
            currentDelta = maxDelta;
    }

    // Si aucune touche de direction n'est pressée, on recentre le volant progressivement
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        if (currentDelta > 0) {
            currentDelta -= deltaIncrementScaled;
            if (currentDelta < 0)
                currentDelta = 0;
        } else if (currentDelta < 0) {
            currentDelta += deltaIncrementScaled;
            if (currentDelta > 0)
                currentDelta = 0;
        }
    }
}

void Car::update(const float dt) {
    handleInput(dt);

    vehicleData vehicle_datas[pointsCount];
    vehicle_datas[0].delta = currentDelta;
    vehicle->getNextIterations(0, pointsCount, vehicle_datas, dt);

    for (int i = 0; i < pointsCount; ++i) {
        carPrediction[i].position = sf::Vector2f(vehicle_datas[i].x, vehicle_datas[i].y);
        carPrediction[i].color = sf::Color::Yellow;
    }

    vehicle->setData(vehicle_datas[0]);
    carSprite.setPosition(vehicle->x, vehicle->y);
    carSprite.setRotation(radToDeg(vehicle->psi));
}


void Car::renderOn(sf::RenderWindow &window) const {
    window.draw(carPrediction);
    window.draw(carSprite);
}
