#ifndef CAR_H
#define CAR_H
#include <deque>
#include <memory>

#include "main_classes.h"

#include <SFML/Graphics.hpp>

#include "vehicle.h"

class Car {
    const Game* game;

    float currentDelta;

    float time = 0.0f; // todo delete;

    sf::Sprite carSprite;

    sf::VertexArray carPrediction;

    /**
     * Here is a buffer of float containing the `dtBufferSize` last values of dt (dt is the delta of time between the frames N-1 and N).<br>
     * It's used to compute the median of the dt to avoid having a too high volatility of the dt.<br>
     * The median of dt is used to calculate the prediction line.
     */
    std::deque<float> dtBuffer;
    const size_t max_dt_buffer_size = 1000;

    /**
     * Get the median of the dtBuffer in a O(n) time complexity.
     * @param dt the delta of time between the frames N-1 and N
     * @return the median of the dtBuffer
     */
    float updateAndGetMedian(const float dt) {
        // O(1) to add the new dt to the deque cf https://en.cppreference.com/w/cpp/container/deque/push_back#:~:text=Complexity-,Constant.,-Exceptions
        dtBuffer.push_back(dt);
        if (dtBuffer.size() > max_dt_buffer_size) {
            // O(1) to remove the first element of the deque cf https://en.cppreference.com/w/cpp/container/deque/pop_front#:~:text=Complexity-,Constant.,-Example
            dtBuffer.pop_front();
        } else {
            printf("dtBuffer size / max: %zu / %zu\n", dtBuffer.size(), max_dt_buffer_size);
        }
        if constexpr (false) { // change this to true to use the old implementation
            /**
             * Implementation v1 => Cost O(nlogn)
             * O(n) to copy dtBuffer to tmp
             * O(nlogn) to sort tmp
             * O(1) to get the value at the index.
             */
            std::vector tmp(dtBuffer.begin(), dtBuffer.end());
            std::sort(tmp.begin(), tmp.end());
            const float median = tmp[tmp.size() / 2];
            return median;
        } else {
            /**
             * Implementation v2 => Cost O(n)
             * O(n) to copy dtBuffer to tmp cf https://en.cppreference.com/w/cpp/container/vector/vector
             * O(n) to use nth_element cf https://en.cppreference.com/w/cpp/algorithm/nth_element
             */
            std::vector tmp(dtBuffer.begin(), dtBuffer.end());
            const auto mid = tmp.begin() + tmp.size() / 2;
            /**
             * nth_element rearranges the elements in two parts separated by the index given in second parameter.
             * like before mid, every element are lower to mid and after mid, every element are greater than mid.
             * but in the indexes [0, mid[ and ]mid, end[ the elements are not sorted.
             */
            std::nth_element(tmp.begin(), mid, tmp.end());
            return *mid.base();
        }
    }

public:
    std::unique_ptr<Vehicle> vehicle;

    /**
     * @param game the game instance
     */
    explicit Car(const Game* game);

    vehicleData computePredictionLine(float dt);

    /**
     * Update the physic and the graphical representation of the car and of the prediction line.
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
     * @return the heading of the car
     */
    float getPsi() const { return vehicle->psi; }

    /**
     * @return the angle of the steering wheel
     */
    float getSteeringAngle() const { return currentDelta; }

    /**
     * @param game_view the view containing the car, the circuit, ...
     * @return true if the car is at the edge of the screen, false otherwise
     */
    bool atTheEdgeOfScreen(const sf::View& game_view) const;

    void recenterViewOnCar(sf::View& game_view, float dt) const;
};

#endif //CAR_H
