#include "car.h"

#include <SFML/Window/Keyboard.hpp>

#include "game.h"
#include "resourcetype.h"
#include "calculationhelper.h"
#include "constants.h"

/**
 * TODO fix the speed increase alone some times ???
 */
Car::Car(const Game* game): game(game), currentDelta(0.0f) {
    constexpr float initSlip = 0.f;
    constexpr float initSlip_tau = 0.2f;
    constexpr float initS_desired = 0.f;
    Vehicle vehicle(
        1500, // mass
        1.4, 1.1, // distance between the center of gravity and the front and rear axles
        0.4, // airResistance coefficient
        100000.0f, // Cx
        40000.0f, // Cy
        1.0f, 0.0f, // vx, vy
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
    constexpr float carScale = 0.5f;
    carSprite.setScale(carScale, carScale);

    carPrediction = sf::VertexArray(sf::LineStrip, pointsCount);
}

/**
 *
 * @param dt the delta of time.
 */
void Car::handleInput(const float dt) {
    constexpr float throttleRate      = 0.01f;     // Taux d'incrémentation par seconde
    constexpr float throttleDecayRate = 0.005f;    // Taux de décroissance par seconde
    constexpr float steeringRate      = 0.15f;     // Taux de changement pour le volant

    const float throttleIncrement = throttleRate * dt;
    const float throttleDecay = throttleDecayRate * dt;
    const float deltaIncrementScaled = steeringRate * dt;


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))           vehicle->s_desired = std::min(vehicle->s_desired + throttleIncrement, 1.0f);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))    vehicle->s_desired = std::max(vehicle->s_desired - throttleIncrement, 0.0f);
    else { // Sinon, si aucune touche n'est pressée, on décroit s_desired vers 0.
        if (vehicle->s_desired > 0.0f) {
            vehicle->s_desired = std::max(vehicle->s_desired - throttleDecay, 0.0f);
        }
    }

    // --- Gestion du volant ---
    constexpr float maxDelta = 0.3f;         // Angle maximal pour le volant
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        currentDelta -= deltaIncrementScaled;
        if (currentDelta < -maxDelta)
            currentDelta = -maxDelta;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        currentDelta += deltaIncrementScaled;
        if (currentDelta > maxDelta)
            currentDelta = maxDelta;
    }
    // Recentrement progressif du volant si aucune touche n'est pressée.
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
    this->time = (time >= 1.0f) ? 0.0f : time + dt;

    handleInput(dt);

    vehicleData vehicle_datas[pointsCount];
    vehicle_datas[0].delta = currentDelta;
    vehicle->getNextIterations(pointsCount, vehicle_datas, dt);

    // Tracé de la trajectoire en appliquant le facteur de conversion
    for (int i = 0; i < pointsCount; ++i) {
        carPrediction[i].position = sf::Vector2f(vehicle_datas[i].x * METER_TO_PIXEL,
                                                  vehicle_datas[i].y * METER_TO_PIXEL);
        carPrediction[i].color = sf::Color::Yellow;
    }

    // Mise à jour de l'état réel du véhicule avec la première itération
    vehicle->setData(vehicle_datas[0]);
    // Positionner le sprite avec conversion
    carSprite.setPosition(vehicle->x * METER_TO_PIXEL, vehicle->y * METER_TO_PIXEL);
    carSprite.setRotation(radToDeg(vehicle->psi));
}


void Car::renderOn(sf::RenderWindow &window) const {
    window.draw(carPrediction);
    window.draw(carSprite);
}

bool Car::atTheEdgeOfScreen(const sf::View &game_view) const {
    const float carX = getX() * METER_TO_PIXEL;
    const float carY = getY() * METER_TO_PIXEL;
    const float viewX = game_view.getCenter().x;
    const float viewY = game_view.getCenter().y;
    const float viewWidth = game_view.getSize().x;
    const float viewHeight = game_view.getSize().y;
    constexpr float deltaScale = 0.9f;
    const float viewLeft = viewX - (viewWidth / 2) * deltaScale;
    const float viewTop = viewY - (viewHeight / 2) * deltaScale;
    const float viewRight = viewX + (viewWidth / 2) * deltaScale;
    const float viewBottom = viewY + (viewHeight / 2) * deltaScale;

    return (carX < viewLeft || carX > viewRight || carY < viewTop || carY > viewBottom);
}

void Car::recenterViewOnCar(sf::View &game_view, const float dt) const {
    sf::Vector2f currentCenter = game_view.getCenter();
    auto targetCenter = sf::Vector2f(getX() * METER_TO_PIXEL, getY() * METER_TO_PIXEL);
    sf::Vector2f offset = targetCenter - currentCenter;
    float distance = std::sqrt(offset.x * offset.x + offset.y * offset.y);

    if (distance > 1.0f) { // seuil pour éviter de trop petites corrections
        // Calcul de la vitesse de la voiture en pixels par seconde
        float carSpeed = std::fabs(getVx()) * METER_TO_PIXEL;
        // On fixe une vitesse minimale pour la caméra pour qu'elle suive même à basse vitesse
        float cameraSpeed = std::max(carSpeed, 200.0f);
        // Distance maximale que la caméra peut parcourir durant ce dt
        float maxDisplacement = cameraSpeed * dt;
        if (maxDisplacement > distance) {
            maxDisplacement = distance;
        }
        // On déplace la caméra d'un incrément proportionnel à l'offset
        sf::Vector2f newCenter = currentCenter + (offset / distance) * maxDisplacement;
        game_view.setCenter(newCenter);
    }
}

