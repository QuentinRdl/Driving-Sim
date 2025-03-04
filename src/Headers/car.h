#ifndef CAR_H
#define CAR_H
#include <memory>

#include "main_classes.h"

#include <SFML/Graphics.hpp>

#include "vehicle.h"

class Car {
    const Game* game;

    float currentDelta;
    float currentSlip;

    const float deltaIncrement = 0.002f; // Increment of angle by frame
    const float slipIncrement  = 0.001f; // Increment of slip by frame
    const float maxDelta = 0.3f;         // Max angle for the steering wheel
    const float maxSlip = 0.2f;          // Max slip (may be negative for the brake)

    sf::Sprite sprite;

public:
    std::unique_ptr<Vehicle> vehicle;
    /**
     * @param game the game instance
     */
    explicit Car(const Game* game);

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

    /**
     * @return the x position of the car
     */
    float getX() const { return vehicle->x; }

    /**
     * @return the y position of the car
     */
    float getY() const { return vehicle->y; }

    /**
     * @return the speed of the car in x direction
     */
    float getVx() const { return vehicle->vx; }
    /**
     * @return the speed of the car in y direction
     */
    float getVy() const { return vehicle->vy; }
    /**
     * @return the yaw rate of the car
     */
    float getLacet() const { return vehicle->r; }
    /**
     * @return the steering angle of the car
     */
    float getSteeringAngle() const { return currentDelta; }
};

#endif //CAR_H
