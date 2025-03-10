#ifndef CAR_H
#define CAR_H
#include <memory>

#include "main_classes.h"

#include <SFML/Graphics.hpp>

#include "vehicle.h"

class Car {
    const Game* game;

    float currentDelta;

    sf::Sprite carSprite;

public:
    std::unique_ptr<Vehicle> vehicle;

    /**
     * @param game the game instance
     */
    explicit Car(const Game* game);

    /**
     * Update the physic and the graphical representation of the car
     * @param dt the delta time
     */
    void update(float dt);

    /**
     * Handle the keyboard input to update the car
     * @param dt the delta time
     */
    void handleInput(float dt);

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
     * @return the direction of the car
     */
    float getPsi() const { return vehicle->psi; }
};

#endif //CAR_H
