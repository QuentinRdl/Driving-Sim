//
// Created by rgld_ on 24/02/25.
//

#ifndef CAR_H
#define CAR_H

#include <SFML/Graphics.hpp>

#include "texturemanager.h"
#include "vehicle.h"

class Car {
    Vehicle vehicle;

    float currentDelta;
    float currentSlip;

    const float deltaIncrement = 0.002f; // Increment of angle by frame
    const float slipIncrement  = 0.001f; // Increment of slip by frame
    const float maxDelta = 0.3f;         // Max angle for the steering wheel
    const float maxSlip = 0.2f;          // Max slip (may be negative for the brake)

    sf::Sprite sprite;
public:


    /**
     * @param tm the texture manager
     * @param vehicle the vehicle (physic model) to use
     */
    Car(const TextureManager& tm, const Vehicle &vehicle);

    /**
     * Handle the keyboard input to update the car 
     */
    void handleInput();

    /**
     * Update the physic and the graphical representation of the car
     * @param dt the delta time
     */
    void update(float dt);

    /**
     * Render the car on the window
     * @param window the window to render the car on
     */
    void renderOn(sf::RenderWindow& window) const;

    float getVx() const { return vehicle.vx; }
    float getVy() const { return vehicle.vy; }
    float getLacet() const { return vehicle.r; }
    float getSteeringAngle() const { return currentDelta; }
};

#endif //CAR_H
